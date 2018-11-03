#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <semaphore.h>

/*
	
    Mutex Bear = 0;
    Semaphore Bees = 0;
    Semaphore Empty = M;
    Mutex Mutex = 1;

    Oso {
        wait(Dormir)
        Repetir M Veces {
            signal(Miel)
        }    
        signal(Producir)
    }
	
    Abeja {
        wait(Producir)
        wait(Miel)
        try_wait(Miel) {
            signal(Miel)
			signal(Producir)
		}
        else {
            signal(Dormir)
        }
	}
	
*/

int const N = 6; //Tengo N Abejas
int const M = 14; //Tengo M espacios para la miel

struct MemoryStruct {
	sem_t Miel; //Si las Abejas pueden hacer wait de este semaforo, es que aun hay espacios vacios
	sem_t Producir; //Mutex que bloquea a las Abejas para que solo pasen de a una
	sem_t Dormir; //Mutex que bloquea al Oso hasta que se llene el tarro
};
typedef struct MemoryStruct Memoria;

Memoria* MemoriaCompartida;

key_t Key;

int shmid;

void Oso () {
	void* p = shmat(shmid, NULL, 0);
	if (p == (void*)-1) {
		perror("Failed ATTACH!");
		exit(1);
	}
	MemoriaCompartida = (Memoria*) p;
	int I;
	while (true) {
		sem_wait(&(MemoriaCompartida->Dormir));
		for (I = 0; I < M; I++) {
			sem_post(&(MemoriaCompartida->Miel));
		}
		sem_post(&(MemoriaCompartida->Producir));
	}
}

void Abeja () {
	void* p = shmat(shmid, NULL, 0);
	if (p == (void*)-1) {
		perror("Failed ATTACH!");
		exit(1);
	}
	MemoriaCompartida = (Memoria*) p;
	int T;
	while (true) {
		sem_wait(&(MemoriaCompartida->Producir));
		sem_wait(&(MemoriaCompartida->Miel));
		T = sem_trywait(&(MemoriaCompartida->Miel));
		if (T > -1) {
			sem_post(&(MemoriaCompartida->Miel));
			sem_post(&(MemoriaCompartida->Producir));
		}
		else {
			sem_post(&(MemoriaCompartida->Dormir));
		}
	}
}

int main () {
	
	int pid;
	int I;
	
	Key = ftok("File",10);
	
	shmid = shmget(Key, sizeof(Memoria), 0660 | IPC_CREAT);
	
	if (shmid == -1) {
		perror("Failed GET!");
		exit(1);
	}
	
	sem_init(&(Memoria->Miel),1,M);
	sem_init(&(Memoria->Producir),1,1);
	sem_init(&(Memoria->Dormir),1,0);
	
	pid = fork();
	
	if (pid == -1) {
		fprintf (stderr,"Error al crear el Proceso Oso");
		exit(1);
	}
	if (pid == 0) {
		Oso();
	}
	if (pid > 0) {
		for (I = 0; I < N; I++){
			pid = fork();
			if (pid == -1) {
				fprintf (stderr,"Error al crear el Proceso Abeja");
				exit(1);
			}
			if (pid == 0) {
				Abeja();
			}
		}
	}
	
	if (pid > 0) {
		wait(NULL);
	}
	
	return 0;
}
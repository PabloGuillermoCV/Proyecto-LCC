#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

//ipcs para listar
//ipcrm -m id para eliminar el segmento

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

int main(int argc, char* argv[]) {

	Memoria* MemoriaCompartida;

	int pid;

	int I;

	key_t Key = generateKey();

	int shmid = shmget(Key, sizeof(Memoria), 0660 | IPC_CREAT);
	if (shmid == -1) {
		perror("Failed GET!");
		exit(1);
	}

	void* P = shmat(shmid, NULL, 0);
	if (P == (void*)-1) {
		perror("Failed ATTACH!");
		exit(1);
	}
	MemoriaCompartida = (Memoria*) P;

	sem_init(&MemoriaCompartida->Miel,1,M);
	sem_init(&MemoriaCompartida->Producir,1,1);
	sem_init(&MemoriaCompartida->Dormir,1,0);

	pid = fork();
	
	if (pid == -1) {
		fprintf (stderr,"Error al crear el Proceso Oso");
		exit(1);
	}
	if (pid == 0) {
		char *args[] = {"./oso",NULL};
		execvp(args[0],args);
	}
	if (pid > 0) {
		for (I = 0; I < N; I++){
			pid = fork();
			if (pid == -1) {
				fprintf (stderr,"Error al crear el Proceso Abeja");
				exit(1);
			}
			if (pid == 0) {
				char *args[] = {"./abeja",NULL};
				execvp(args[0],args);
			}
		}
	}

	if (pid > 0) {
		for (I = 1; I <= N+1; I++) {
			wait(NULL);
		}
	}

	if (shmdt(P) != 0) {
		perror("Failed DETTACH!");
		exit(1);
	}

	return 0;
}
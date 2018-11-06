#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main () {
	
	Memoria* MemoriaCompartida;
	
	int T;
	
	int shmid = shmget(generateKey(), sizeof(Memoria), 0660);
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
	
	while (true) {
		sem_wait(&MemoriaCompartida->Producir);
		sem_wait(&MemoriaCompartida->Miel);
		T = sem_trywait(&MemoriaCompartida->Miel);
		if (T == 0) {
			sem_post(&MemoriaCompartida->Miel);
			printf("Se produjo miel y quedan espacios, pasa la siguiente Abeja.\n");
			sem_post(&MemoriaCompartida->Producir);
		}
		else {
			printf("Se produjo miel y se lleno el tarro, se despierta al Oso.\n");
			sem_post(&MemoriaCompartida->Dormir);
		}
	}
	
	return 0;
}
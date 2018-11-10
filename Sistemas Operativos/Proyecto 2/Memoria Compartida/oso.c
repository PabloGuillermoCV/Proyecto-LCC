#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main () {

	Memoria* MemoriaCompartida;

	int I;

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
		sem_wait(&MemoriaCompartida->Dormir);
		for (I = 0; I < M; I++) {
			sem_post(&MemoriaCompartida->Miel);
		}
		printf("El Oso consumio toda la miel.\n");
		sem_post(&MemoriaCompartida->Producir);
	}

	return 0;
}
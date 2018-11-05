#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main () {

	Memoria* MemoriaCompartida;

	int shmid = shmget(generateKey(), sizeof(Memoria), 0660);
	if (shmid == -1) {
		perror("Failed GET!"); exit(1);
	}

	void* P = shmat(shmid, NULL, 0);
	if (P == (void*)-1) {
		perror("Failed ATTACH!"); exit(1);
	}
	MemoriaCompartida = (Memoria*) P;

	int T;

	while (true) {
		sem_wait(&MemoriaCompartida->Producir);
		sem_wait(&MemoriaCompartida->Miel);
		T = sem_trywait(&MemoriaCompartida->Miel);
		if (T == 0) {
			sem_post(&MemoriaCompartida->Miel);
			sem_post(&MemoriaCompartida->Producir);
		}
		else {
			sem_post(&MemoriaCompartida->Dormir);
		}
	}

	return 0;
}
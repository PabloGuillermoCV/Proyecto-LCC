#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main(int argc, char* argv[]) {
	key_t key = generateKey();

	int shmid = shmget(key, sizeof(tMemory), 0660 | IPC_CREAT);
	if (shmid == -1) {
		perror("Failed GET!"); exit(1);
	}
	printf("SHMID=%d\n", shmid);

	void* p = shmat(shmid, NULL, 0);
	if (p == (void*)-1) {
		perror("Failed ATTACH!"); exit(1);
	}
	tMemory* sharedMemory = (tMemory*) p;

	sem_init(&sharedMemory->full, 1, 0);
	sem_init(&sharedMemory->empty, 1, BUFFERSIZE);
	sem_init(&sharedMemory->mutex, 1, 1);
	sharedMemory->last = -1;

	if (shmdt(p) != 0) {
		perror("Failed DETTACH!"); exit(1);
	}
}
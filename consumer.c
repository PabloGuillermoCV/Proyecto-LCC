#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

tMemory* sharedMemory;
int itemNumber = 0;


void consume(tBike bike) {
	int duration = rand() % 5;
	printf("Consuming bike #%d with Color: '%c' Size: %d, produced by [%d].\n", itemNumber++, bike.color, bike.size, bike.creator);
	sleep(duration);
}

void getFromBuffer(tBike* bike) {
	sem_wait(&sharedMemory->mutex);
	printf("Getting from buffer...");

	*bike = sharedMemory->buffer[0];

	int i;
	int newLast = sharedMemory->last - 1;
	for (i = 0; i < newLast; i++) {
		sharedMemory->buffer[i] = sharedMemory->buffer[i + 1];
	}
	sharedMemory->last = newLast;

	printf("Retrieved!\n");
	sem_post(&sharedMemory->mutex);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	int shmid = shmget(generateKey(), sizeof(tMemory), 0660);
	if (shmid == -1) {
		perror("Failed GET!"); exit(1);
	}
	printf("SHMID=%d\n", shmid);

	void* p = shmat(shmid, NULL, 0);
	if (p == (void*)-1) {
		perror("Failed ATTACH!"); exit(1);
	}
	sharedMemory = (tMemory*) p;

	tBike bike;
	while(1) {
		printf("\n[%ld] Waiting full cell...\n", time(NULL));
		sem_wait(&sharedMemory->full);
		getFromBuffer(&bike);
		printf("Signalling empty cell\n");
		sem_post(&sharedMemory->empty);

		consume(bike);
	}
}
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

char COLORS[] = {'R', 'G', 'B', 'C', 'M', 'Y', 'K'};
int SIZES[] = {20, 24, 26, 29};

tMemory* sharedMemory;
int itemNumber = 0;

void produce(tBike* bike) {
	int duration = rand() % 5;
	int colorIndex = rand() % 7;
	int sizeIndex = rand() % 4;

	bike->color = COLORS[colorIndex];
	bike->size = SIZES[sizeIndex];
	bike->creator = getpid();
	printf("\n[%d] Producing bike #%d with Color: '%c' Size: %d.\n", bike->creator, itemNumber++, bike->color, bike->size);
	sleep(duration);
}

void saveInBuffer(tBike bike) {
	sem_wait(&sharedMemory->mutex);
	printf("Saving into buffer...");

	int newLast = sharedMemory->last + 1;
	sharedMemory->buffer[newLast] = bike;
	sharedMemory->last = newLast;

	printf("Saved!\n");
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
		produce(&bike);

		printf("Waiting empty cell...\n");
		sem_wait(&sharedMemory->empty);
		saveInBuffer(bike);
		printf("Signalling full cell\n");
		sem_post(&sharedMemory->full);
	}

	return 0;
}
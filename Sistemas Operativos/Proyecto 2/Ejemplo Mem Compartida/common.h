#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

#define BUFFERSIZE 10

struct Bike {
	pid_t creator;
	char color; // 'R', 'G', 'B', 'C', 'M', 'Y', 'K'
	int size;   // 20, 24, 26, 29
};
typedef struct Bike tBike;

struct Memory {
	sem_t full;
	sem_t empty;
	sem_t mutex;
	tBike buffer[BUFFERSIZE];
	int last;
};
typedef struct Memory tMemory;

key_t generateKey() {
	int projectId = 14;
	char path[200];
	getcwd(path, 200 * sizeof(char));
	key_t key = ftok(path, projectId);
	printf("Generated key for path '%s': %d\n", path, key);
	return key;
}
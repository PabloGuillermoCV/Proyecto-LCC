#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

int const M = 14; //Tengo M espacios para la miel

struct MemoryStruct {
	sem_t Miel; //Si las Abejas pueden hacer wait de este semaforo, es que aun hay espacios vacios
	sem_t Producir; //Mutex que bloquea a las Abejas para que solo pasen de a una
	sem_t Dormir; //Mutex que bloquea al Oso hasta que se llene el tarro
};
typedef struct MemoryStruct Memoria;

key_t generateKey() {
	key_t Key = ftok("File",10);
	return Key;
}
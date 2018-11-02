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

#define N 6
#define M 14

/*
	
	EspacioVacio = M
	Producir = 1
	Dormir = 0
	
	Abeja:
		Wait(Producir)
		Wait(EspacioVacio)
		Trywait(EspacioVacio)
			Signal(EspacioVacio)
			Signal(Producir)
		else
			Signal(Dormir)
	
	Oso:
		Wait(Dormir)
		Repetir M Veces:
			Signal(EspacioVacio)
		Signal(Producir)
	
*/

key_t Key;

struct Buffer_M { 
    long Tipo; 
    char Texto;
} Mensaje;

void Oso () {
	while (true) {
		
	}
}

void Abeja () {
	while (true) {
		
	}
}

int main () {
	
	Key = ftok("File",10);
	
	int pid;
	int I;
	
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
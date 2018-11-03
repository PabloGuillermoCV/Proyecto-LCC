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

//Tipo 1: Abeja produce miel (recibe el mensaje) y Oso consume miel (envia el mensaje)
//Tipo 2: No hay miel (En el caso de que una Abeja no recibe un mensaje de tipo 1, para despertar al Oso)
//Tipo 4: Abeja entra a producir miel (Recibido por las Abejas y es enviado por otra Abeja o por un Oso cuando termina de consumir la miel)
//Tipo 6: Despertar Oso (Abeja envia cuando recibe un mensaje de tipo 2 y Oso recibe)

/*
	
	El ciclo comienza con el Oso enviando M mensajes de tipo 1, uno de tipo 2 y otro de tipo 4, antes de entrar al while
	
	Oso:
		Recibe Mensaje Tipo 6;
		Envia M Mensajes Tipo 1;
		Envia Mensaje Tipo 2;
		Envia Mensaje Tipo 4;
	
	Abeja:
		Recibe Mensaje Tipo 4;
		Recibe Mensaje Tipo -2:
		Si Recibe Mensaje Tipo 1;
			Envia Mensaje Tipo 4;
		Si Recibe Mensaje Tipo 2;
			Envia Mensaje Tipo 6;
	
*/

int const N = 6;
int const M = 3;

key_t Key;

struct Buffer_M { 
    long Tipo; 
    char Texto;
} Mensaje;

void Oso () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	int I;
	for (I = 0; I < M; I++) {
		Mensaje.Tipo = 1;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
	
	Mensaje.Tipo = 2;
	Mensaje.Texto = 'X';
	msgsnd(MsgID,&Mensaje,1,0666);
	
	Mensaje.Tipo = 4;
	Mensaje.Texto = 'X';
	msgsnd(MsgID,&Mensaje,1,0666);
	
	while (true) {
		msgrcv(MsgID,&Mensaje,1,6,0666);
		for (I = 0; I < M; I++) {
			Mensaje.Tipo = 1;
			Mensaje.Texto = 'X';
			msgsnd(MsgID,&Mensaje,1,0666);
		}
		
		Mensaje.Tipo = 2;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		printf("El Oso consumio toda la miel.\n");
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void Abeja () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,4,0666);
		msgrcv(MsgID,&Mensaje,1,-2,0666);
		
		if (Mensaje.Tipo == 1) {
			printf("Se produjo miel, pasa la siguiente Abeja.\n");
			Mensaje.Tipo = 4;
			Mensaje.Texto = 'X';
			msgsnd(MsgID,&Mensaje,1,0666);
		}
		
		else {
			if (Mensaje.Tipo == 2) {
				printf("No se puede poner mas miel, se despierta al Oso.\n");
				Mensaje.Tipo = 6;
				Mensaje.Texto = 'X';
				msgsnd(MsgID,&Mensaje,1,0666);
			}
		}
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
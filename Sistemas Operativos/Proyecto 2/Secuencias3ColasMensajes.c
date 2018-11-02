#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h>

//ACDE BCDE ABCDE  ACDE BCDE ABCDE...

/*
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 3; Recibe Mensaje 1; Imprime A; Envia Mensaje 2;
	
	B Hace: Recibe Mensaje 2; Imprime B; Envia Mensaje 3;
	
	C Hace: Recibe Mensaje 3; Imprime C; Envia Mensaje 4;
	
	D Hace: Recibe Mensaje 4; Imprime D; Envia Mensaje 5;
	
	E Hace: Recibe Mensaje 5; Imprime E; Envia Mensaje 2; Recibe Mensaje 5; Imprime E; Envia Mensaje 1; Recibe Mensaje 5; Imprime E; Envia Mensaje 1;
	
*/

key_t Key;

struct Buffer_M { 
    long Tipo; 
    char Texto;
} Mensaje; 

void A () { //Lee los mensajes de tipo 1
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("A");
		Mensaje.Tipo = 3;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("A");
		Mensaje.Tipo = 2;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,10,0666);
	}
}

void B () { //Lee los mensajes de tipo 2
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,2,0666);
		printf("B");
		Mensaje.Tipo = 3;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void C () { //Lee los mensajes de tipo 3
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,3,0666);
		printf("C");
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void D () { //Lee los mensajes de tipo 4
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,4,0666);
		printf("D");
		Mensaje.Tipo = 5;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void E () { //Lee los mensajes de tipo 5
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,5,0666);
		printf("E");
		Mensaje.Tipo = 2;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		msgrcv(MsgID,&Mensaje,1,5,0666);
		printf("E");
		Mensaje.Tipo = 1;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		msgrcv(MsgID,&Mensaje,1,5,0666);
		printf("E");
		Mensaje.Tipo = 1;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

int main () {
	
	Key = ftok("File",10);
	
	int pid = fork ();
    if (pid == -1) {
    	fprintf (stderr,"Error al crear el Proceso Hijo A");
    }
    else {
    	if (pid == 0) {
        	pid = fork ();
			if (pid == -1) {
				fprintf (stderr,"Error al crear el Proceso Hijo B");
			}
			else {
				if (pid != 0) A ();
				if (pid == 0) {
					pid = fork ();
					if (pid == -1) {
						fprintf (stderr,"Error al crear el Proceso Hijo C");
					}
					else {
						if (pid != 0) B ();
						if (pid == 0) {
							pid = fork ();
							if (pid == -1) {
								fprintf (stderr,"Error al crear el Proceso Hijo D");
							}
							else {
								if (pid != 0) C ();
								if (pid == 0) {
									pid = fork ();
									if (pid == -1) {
										fprintf (stderr,"Error al crear el Proceso Hijo E");
									}
									else {
										if (pid != 0) D ();
										if (pid == 0) {
											E ();
										}
									}
								}
							}
						}
					}
				}
			}
    	}
		exit(0);
    }

	if (pid != 0){
    	wait(NULL);
	}
	
	return 0;
}
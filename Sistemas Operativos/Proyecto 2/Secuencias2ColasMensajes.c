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

//(AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE...

/*
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 4;
	
	B Hace: Recibe Mensaje 1; Imprime B; Envia Mensaje 4;
	
	C Hace: Recibe Mensaje 1; Imprime C; Envia Mensaje 4;
	
	D Hace: Recibe Mensaje 6; Recibe Mensaje 4; Imprime D; Envia Mensaje 5;
	
	E Hace: Recibe Mensaje 4; Envia Mensaje 6; Envia Mensaje 1; Recibe Mensaje 5; Imprime E; Envia Mensaje 1;
	
*/

key_t Key;

struct Buffer_M { 
    long Tipo; 
    char Texto;
} Mensaje; 

void A () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("A");
		
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void B () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("B");
		
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void C () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("C");
		
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void D () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		//D espera a que E le avise que puede recibir el mensaje de (AoBoC)
		msgrcv(MsgID,&Mensaje,1,6,0666);
		
		//D espera el mensaje del segundo (AoBoC)
		msgrcv(MsgID,&Mensaje,1,4,0666);
		printf("D");
		
		//D envia un mensaje a E
		Mensaje.Tipo = 5;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void E () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	
	//E envia un mensaje a (AoBoC) para comenzar el ciclo
	Mensaje.Tipo = 1;
	Mensaje.Texto = 'X';
	msgsnd(MsgID,&Mensaje,1,0666);
	
	while (true) {
		//E es notificado que ABC termino por pimera vez
		msgrcv(MsgID,&Mensaje,1,4,0666);
		
		//Le avisa a D para que en el siguiente ciclo reciba el mensaje
		Mensaje.Tipo = 6;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		//E envia un mensaje a (AoBoC)
		Mensaje.Tipo = 1;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		//E recibe un mensaje de D e imprime
		msgrcv(MsgID,&Mensaje,1,5,0666);
		printf("E");
		
		//E vuelve a enviar un mensaje a (AoBoC)
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
			exit(0);
    	}
    }

	if (pid > 0){
    	wait(NULL);
	}
	
	return 0;
}
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h>

//(AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE...

/*
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 4;
	
	B Hace: Recibe Mensaje 2; Imprime B; Envia Mensaje 4;
	
	C Hace: Recibe Mensaje 3; Imprime C; Envia Mensaje 4;
	
	D Hace: Recibe Mensaje 4; Imprime D; Envia Mensaje 5; Recibe Mensaje 6;
	
	E Hace: Recibe Mensaje 5; Imprime E; Envia Mensaje (1o2o3); Recibe Mensaje 4; Envia Mensaje 6; Envia Mensaje (1o2o3);
	
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
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void B () { //Lee los mensajes de tipo 2
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,2,0666);
		printf("B");
		Mensaje.Tipo = 4;
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

void D () { //Lee los mensajes de tipo 4 y 6
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,4,0666);
		printf("D");
		//D envia un mensaje a E
		Mensaje.Tipo = 5;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		//D espera a que el grupo ABC pase dos veces
		//E le envia un mensaje avisandole cuando puede seguir
		msgrcv(MsgID,&Mensaje,1,6,0666);
	}
}

void E () { //Lee los mensajes de tipo 4 y 5
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,5,0666);
		printf("E");
		//E envia un mensaje aleatorio a ABC
		int NumRandom = rand() % 3 + 1;
		Mensaje.Tipo = NumRandom;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
		
		//E es notificado que ABC termino por pimera vez
		//Le avisa a D para que en el siguiente ciclo reciba el mensaje
		msgrcv(MsgID,&Mensaje,1,4,0666);
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		
		//Vuelve a enviar un mensaje aleatorio a ABC
		NumRandom = rand() % 3 + 1;
		Mensaje.Tipo = NumRandom;
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
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//ACDE BCDE ABCDE  ACDE BCDE ABCDE...

/*
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 3; Recibe Mensaje 1; Imprime A; Envia Mensaje 2;
	
	B Hace: Recibe Mensaje 2; Imprime B; Envia Mensaje 3;
	
	C Hace: Recibe Mensaje 3; Imprime C; Envia Mensaje 4;
	
	D Hace: Recibe Mensaje 4; Imprime D; Envia Mensaje 5;
	
	E Hace: Recibe Mensaje 5; Imprime E; Envia Mensaje 2; Recibe Mensaje 5; Imprime E; Envia Mensaje 1; Recibe Mensaje 5; Imprime E; Envia Mensaje 1;
	
*/

void A () { //Lee los mensajes de tipo 1
	int MsgID = msgget(Key, 0666 | IPC_CREAT & IPC_NOWAIT);
	while (true) {
		msgrcv(msgID,Mensaje,10,1,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 3;
		Mensaje.Texto = "C";
		msgsnd(msgID,Mensaje,10,0666);
		
		msgrcv(msgID,Mensaje,10,1,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 2;
		Mensaje.Texto = "B";
		msgsnd(msgID,Mensaje,10,0666);
	}
}

void B () { //Lee los mensajes de tipo 2
	int MsgID = msgget(Key, 0666 | IPC_CREAT & IPC_NOWAIT);
	while (true) {
		msgrcv(msgID,Mensaje,10,2,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 3;
		Mensaje.Texto = "C";
		msgsnd(msgID,Mensaje,10,0666);
	}
}

void C () { //Lee los mensajes de tipo 3
	int MsgID = msgget(Key, 0666 | IPC_CREAT & IPC_NOWAIT);
	while (true) {
		msgrcv(msgID,Mensaje,10,3,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 4;
		Mensaje.Texto = "D";
		msgsnd(msgID,Mensaje,10,0666);
	}
}

void D () { //Lee los mensajes de tipo 4
	int MsgID = msgget(Key, 0666 | IPC_CREAT & IPC_NOWAIT);
	while (true) {
		msgrcv(msgID,Mensaje,10,4,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 5;
		Mensaje.Texto = "E";
		msgsnd(msgID,Mensaje,10,0666);
	}
}

void E () { //Lee los mensajes de tipo 5
	int MsgID = msgget(Key, 0666 | IPC_CREAT & IPC_NOWAIT);
	while (true) {
		msgrcv(msgID,Mensaje,10,5,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 2;
		Mensaje.Texto = "B";
		msgsnd(msgID,Mensaje,10,0666);
		
		msgrcv(msgID,Mensaje,10,5,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 1;
		Mensaje.Texto = "A";
		msgsnd(msgID,Mensaje,10,0666);
		
		msgrcv(msgID,Mensaje,10,5,0666);
		printf(Mensaje.Texto);
		Mensaje.Tipo = 1;
		Mensaje.Texto = "A";
		msgsnd(msgID,Mensaje,10,0666);
	}
}

int main () {
	
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
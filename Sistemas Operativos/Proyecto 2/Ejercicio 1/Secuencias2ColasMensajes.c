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
	
	El Padre comienza enviando un mensaje 1;
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 4;

	B Hace: Recibe Mensaje 1; Imprime B; Envia Mensaje 4;

	C Hace: Recibe Mensaje 1; Imprime C; Envia Mensaje 4;

	D Hace: Recibe Mensaje 2; Recibe Mensaje 4; Imprime D; Envia Mensaje 3;

	E Hace: Recibe Mensaje 4; Envia Mensaje 2; Envia Mensaje 1; Recibe Mensaje 3; Imprime E; Envia Mensaje 1;

*/

//Tipos de mensajes
#define tABC 1
#define tD 2
#define tE 3
#define tDE 4

key_t Key;

struct Buffer_M {
    long Tipo;
    char Texto;
};

typedef struct Buffer_M Msg;

int getQueue () { //Usado para conectar a la cola de mensajes (ya creada)
	int ID = msgget (Key, 0666);
	if (ID == -1){
		perror ("Error - msgget: ");
	}
	return ID;
}

void send (int qid, int tipo) { //Usado para enviar mensajes
    Msg M;
    M.Tipo = tipo;
    M.Texto = 'X';
	fflush (NULL);
	int Result = msgsnd (qid,&M,1,0666);
	fflush (NULL);
    if (Result == -1) {
        perror ("Error - msgsnd: ");
        exit(1);
    }
}

void receive (int qid, int tipo) { //Usado para recibir mensajes
    Msg M;
	fflush (NULL);
	int Result = msgrcv (qid,&M,1,tipo,0666);
	fflush (NULL);
    if (Result == -1){
        perror ("Error - msgrcv: ");
        exit(1);
    }
}

void A () {
	int MsgID = getQueue ();
	while (true) {
		//A recibe un mensaje de E
		receive (MsgID,tABC);
		printf ("A");
		
		//A envia un mensaje a D o E
		send (MsgID,tDE);
	}
}

void B () {
	int MsgID = getQueue ();
	while (true) {
		//B recibe un mensaje de E
		receive (MsgID,tABC);
		printf ("B");
		
		//B envia un mensaje a D o E
		send (MsgID,tDE);
	}
}

void C () {
	int MsgID = getQueue ();
	while (true) {
		//C recibe un mensaje de E
		receive (MsgID,tABC);
		printf ("C");
		
		//C envia un mensaje a D o E
		send (MsgID,tDE);
	}
}

void D () {
	int MsgID = getQueue ();
	while (true) {
		//D espera a que E le avise que puede recibir el mensaje de (AoBoC)
		receive (MsgID,tD);

		//D espera el mensaje del segundo (AoBoC)
		receive (MsgID,tDE);
		printf ("D");

		//D envia un mensaje a E
		send (MsgID,tE);
	}
}

void E () {
	int MsgID = getQueue ();
	while (true) {
		//E es notificado que ABC termino por pimera vez
		receive (MsgID,tDE);

		//Le avisa a D para que en el siguiente ciclo reciba el mensaje
		send (MsgID,tD);

		//E envia un mensaje a (AoBoC)
		send (MsgID,tABC);

		//E recibe un mensaje de D e imprime
		receive (MsgID,tE);
		printf ("E");
		
		//E envia un mensaje a (AoBoC)
		send (MsgID,tABC);
	}
}

void dispatch (int I) {
	switch (I) {
        case 1: A(); break;
        case 2: B(); break;
        case 3: C(); break;
        case 4: D(); break;
        case 5: E(); break;
    }
}

int main () {

	Key = ftok (".",10);
	
	//Se crea la cola de mensajes
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	if (MsgID == -1) {
        perror ("Padre.mssget - Error: ");
        exit (1);
	}

    pid_t pid = NULL;
	
    int I;
    int cantP = 5;
	//Se crean los 5 procesos A, B, C, D y E
    for (I = 1; I <= cantP; I++){
        pid = fork ();
        if (pid == -1) {
			fprintf (stderr,"Error al crear el Proceso");
		}
        if (pid == 0){
            dispatch (I);
            exit (0);
        }
    }
	
	//El padre envia un mensaje a (AoBoC) para empezar el ciclo
	send (MsgID,tABC);
	
	for (I = 1; I <= cantP; I++) {
        wait (NULL);
    }

	return 0;
}
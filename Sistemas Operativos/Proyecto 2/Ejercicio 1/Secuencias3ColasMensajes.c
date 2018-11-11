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

//ACDE BCDE ABCDE  ACDE BCDE ABCDE...

/*
	
	El Padre comienza enviando un mensaje 1;
	
	A Hace: Recibe Mensaje 1; Imprime A; Envia Mensaje 3; Recibe Mensaje 1; Imprime A; Envia Mensaje 2;

	B Hace: Recibe Mensaje 2; Imprime B; Envia Mensaje 3;

	C Hace: Recibe Mensaje 3; Imprime C; Envia Mensaje 4;

	D Hace: Recibe Mensaje 4; Imprime D; Envia Mensaje 5;

	E Hace: Recibe Mensaje 5; Imprime E; Envia Mensaje 2; Recibe Mensaje 5; Imprime E; Envia Mensaje 1; Recibe Mensaje 5; Imprime E; Envia Mensaje 1;

*/

//Tipos de mensajes
#define tA 1
#define tB 2
#define tC 3
#define tD 4
#define tE 5

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
		receive (MsgID,tA);
		printf ("A");

		//A envia un mensaje a C
		send (MsgID,tC);
		
		//A recibe un mensaje de E
		receive (MsgID,tA);
		printf ("A");

		//A envia un mensaje a B
		send (MsgID,tB);
	}
}

void B () {
	int MsgID = getQueue ();
	while (true) {
		//B recibe un mensaje de A o E
		receive (MsgID,tB);
		printf ("B");

		//B envia un mensaje a C
		send (MsgID,tC);
	}
}

void C () {
	int MsgID = getQueue ();
	while (true) {
		//C recibe un mensaje de A o B
		receive (MsgID,tC);
		printf ("C");

		//C envia un mensaje a D
		send (MsgID,tD);
	}
}

void D () {
	int MsgID = getQueue ();
	while (true) {
		//D recibe un mensaje de C
		receive (MsgID,tD);
		printf ("D");

		//D envia un mensaje a E
		send (MsgID,tE);
	}
}

void E () {
	int MsgID = getQueue ();
	while (true) {
		//E recibe un mensaje de D
		receive (MsgID,tE);
		printf ("E");

		//E le envia un mensaje a B para imprimir la parte BCDE
		send (MsgID,tB);
		
		//E recibe un mensaje de D
		receive (MsgID,tE);
		printf ("E");

		//E le envia un mensaje a A para imprimir la parte ABCDE
		send (MsgID,tA);
		
		//E recibe un mensaje de D
		receive (MsgID,tE);
		printf ("E");
		
		//E le envia un mensaje a A para imprimir la parte ACDE
        send (MsgID,tA);
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
	
	//El padre envia un mensaje a A para empezar el ciclo
	send (MsgID,tA);
	
    for (I = 1; I <= cantP; I++) {
        wait (NULL);
    }

	return 0;
}
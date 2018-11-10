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

// Tipo de mensajes
#define tA 1
#define tB 2
#define tC 3
#define tD 4
#define tE 5

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


typedef struct Buffer_M Msg;

int getQueue(){
  int id = msgget(Key, 0666);
  if (id == -1){
    perror("Error - msgget: ");
  }
  return id;
}

void send(int qid, int tipo) {
    Msg M;

    M.Tipo = tipo;
    M.Texto = 'X';
    if (msgsnd(qid,&M,1,0666) == -1) {
        perror("Error - msgsnd: ");
        exit(1);
    }
}

void rcv(int qid, int tipo) {
    Msg M;
    if (msgrcv(qid,&M,1,tipo,0666) == -1){
        perror("Error - msgrcv: ");
        exit(1);
    }
}

void A () {
 fprintf(stdout,"A: inicio");

	int MsgID = getQueue();

 fprintf(stdout,"A: get queue");

	while (true) {

 fprintf(stdout,"A: espero por tipo 1");
		rcv(MsgID,1);

 fprintf(stdout,"A: Recibi tipo 1 - END");
		printf("A");

		//A envia un mensaje a C
		Mensaje.Tipo = 3;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);

		msgrcv(MsgID,&Mensaje,1,1,0666);
		printf("A");

		//A envia un mensaje a B
		Mensaje.Tipo = 2;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void B () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,2,0666);
		printf("B");

		//B envia un mensaje a C
		Mensaje.Tipo = 3;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void C () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,3,0666);
		printf("C");

		//C envia un mensaje a D
		Mensaje.Tipo = 4;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void D () {
	int MsgID = msgget(Key, 0666 | IPC_CREAT);
	while (true) {
		msgrcv(MsgID,&Mensaje,1,4,0666);
		printf("D");

		//D envia un mensaje a E
		Mensaje.Tipo = 5;
		Mensaje.Texto = 'X';
		msgsnd(MsgID,&Mensaje,1,0666);
	}
}

void E () {

 fprintf(stdout,"E: init");
	int MsgID = getQueue();

 fprintf(stdout,"E: get queue");
	while (true) {
        //E le envia un mensaje a A para empezar el ciclo con ACDE
        fprintf(stdout,"E: Envio tipo 1");
        send(MsgID, 1);
 fprintf(stdout,"E: Envio tipo 1 - END");

		int X = msgrcv(MsgID,&Mensaje,1,5,0666);
		if (X == -1) {
            perror("E.msgsnd - Error: ");
            exit(1);
        }
		fprintf(stdout,"E");

		//E le envia un mensaje a B para imprimir la parte BCDE
		Mensaje.Tipo = 2;
		Mensaje.Texto = 'X';
		X = msgsnd(MsgID,&Mensaje,1,0666);
		if (X == -1) {
            perror("E.msgsnd - Error: ");
            exit(1);
        }

		X = msgrcv(MsgID,&Mensaje,1,5,0666);
		if (X == -1) {
            perror("E.msgsnd - Error: ");
            exit(1);
        }
		fprintf(stdout,"E");

		//E le envia un mensaje a B para imprimir la parte ABCDE
		Mensaje.Tipo = 1;
		Mensaje.Texto = 'X';
		X = msgsnd(MsgID,&Mensaje,1,0666);
		if (X == -1) {
            perror("E.msgsnd - Error: ");
            exit(1);
        }

		X = msgrcv(MsgID,&Mensaje,1,5,0666);
		if (X == -1) {
            perror("E.msgsnd - Error: ");
            exit(1);
        }
		fprintf(stdout,"E");
	}
}

void dispatch(int i) {
        switch (i) {
            case tA: A(); break;
            //case tB: B(); break;
            //case tC: C(); break;
            //case tD: D(); break;
            case tB: E(); break;
        }
}

int main () {

	Key = ftok(".",10);
    int MsgID = msgget(Key, 0666 | IPC_CREAT);
	if (MsgID == -1) {
        perror("Padre.mssget - Error: ");
        exit(1);
	}

    pid_t p=NULL;
    int i;
    int cant = 2;
    for (i=1;i<=cant;i++){
        p = fork();
        if (p == -1) {
			fprintf (stderr,"Error al crear el Proceso");
		}
        if (p == 0){
            dispatch(i);
            exit(0);
        }
    }

    for (i=1;i<=cant;i++){
        wait(NULL);
    }

	return 0;
}

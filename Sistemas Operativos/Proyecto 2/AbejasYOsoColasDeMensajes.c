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

/*
	
	El ciclo comienza con el Padre enviando M-1 mensajes de tipo 1, uno de tipo 2 y otro de tipo 4;
	
	Oso:
		Recibe Mensaje Tipo 6;
		Envia M-1 Mensajes Tipo 1;
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

int const N = 6; //Tengo N Abejas
int const M = 14; //Tengo M espacios para la miel

//Tipos de mensajes
#define tMiel 1
#define tUlt 2
#define tProd 4
#define tOso 6

//Tipo 1: Abeja produce miel (recibe un mensaje) y Oso consume miel (envia M-1 mensajes)
//Tipo 2: Ultimo espacio libre de miel (lo recibe la ultima Abeja, la cual se encarga de llamar al Oso) (El Oso envia este mensaje despues de M-1 mensajes de tipo 1)
//Tipo 4: Abeja entra a producir miel (Recibido por las Abejas y es enviado por otra Abeja o por un Oso cuando termina de consumir la miel)
//Tipo 6: Despertar Oso (Abeja envia cuando recibe un mensaje de tipo 2 y Oso recibe)

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

int receive (int qid, int tipo) { //Usado para recibir mensajes
    Msg M;
	fflush (NULL);
	int Result = msgrcv (qid,&M,1,tipo,0666);
	fflush (NULL);
    if (Result == -1){
        perror ("Error - msgrcv: ");
        exit(1);
    }
	return M.Tipo;
}

void Oso () {
	int MsgID = getQueue ();
	int I;
	while (true) {
		//El Oso recibe un mensaje de una Abeja cuando se lleno el tarro
		receive (MsgID,tOso);
		
		for (I = 0; I < M-1; I++) {
			//El Oso enviando un mensaje de tipo 1 significa que consumio un espacio del tarro
			send (MsgID,tMiel);
		}
		
		//El Oso envia un mensaje 2 el cual simboliza el ultimo espacio libre del tarro, ubicado al final de la cola
		send (MsgID,tUlt);
		
		printf ("El Oso consumio toda la miel.\n");
		//El Oso vacio el tarro, envia un mensaje a una Abeja para que pueda entrar a producir
		send (MsgID,tProd);
	}
}

void Abeja () {
	int MsgID = getQueue ();
	int Tipo;
	while (true) {
		//La Abeja recibe un mensaje de el Oso u otra Abeja para entrar a producir
		receive (MsgID,tProd);
		
		//La Abeja recibe un mensaje que puede o no ser del ultimo espacio libre en el trarro
		Tipo = receive (MsgID,-tUlt);
		
		if (Tipo == tMiel) {
			printf ("Se produjo miel y quedan espacios, pasa la siguiente Abeja.\n");
			//Si no era el ultimo espacio, envia un mensaje a otra Abeja para que entre a producir
			send (MsgID,tProd);
		}
		
		else {
			if (Tipo == tUlt) {
				printf ("Se produjo miel y se lleno el tarro, se despierta al Oso.\n");
				//Si era el ultimo espacio disponible y ahora esta lleno, envia un mensaje al Oso para despertarlo
				send (MsgID,tOso);
			}
		}
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
	
	//Se crea 1 proceso Oso
	pid = fork ();
	if (pid == -1) {
		fprintf (stderr,"Error al crear el Proceso Oso");
		exit (1);
	}
	if (pid == 0) {
		Oso ();
		exit (0);
	}
	
	if (pid > 0) {
		//Se crean los N procesos Abejas
		for (I = 0; I < N; I++){
			pid = fork ();
			if (pid == -1) {
				fprintf (stderr,"Error al crear el Proceso Abeja");
				exit (1);
			}
			if (pid == 0) {
				Abeja ();
				exit (0);
			}
		}
	}
	
	//Se inicializan los M espacios para producir miel
	for (I = 0; I < M-1; I++) {
		send (MsgID,tMiel);
	}
	send (MsgID,tUlt);
	//Se envia un mensaje para que la primera Abeja pueda entrar
	send (MsgID,tProd);
	
	//El proceso Padre espera por las N Abejas y 1 Oso
	for (I = 1; I <= N+1; I++) {
		wait(NULL);
	}
	
	return 0;
}
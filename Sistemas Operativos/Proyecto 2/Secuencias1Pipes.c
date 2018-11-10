#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

//ABCDE ABCDE ABCDE ABCDE...

/*
	
	A Hace: Recibe Mensaje PipeA; Imprime A; Envia Mensaje PipeB;
	
	B Hace: Recibe Mensaje PipeB; Imprime B; Envia Mensaje PipeC;
	
	C Hace: Recibe Mensaje PipeC; Imprime C; Envia Mensaje PipeD;
	
	D Hace: Recibe Mensaje PipeD; Imprime D; Envia Mensaje PipeE;
		
	E Hace: Recibe Mensaje PipeE; Imprime E; Envia Mensaje PipeA;
	
*/

//0 es para leer y 1 es para escribir

char writeMessage;
char readMessage;

int PipeA[2];
int PipeB[2];
int PipeC[2];
int PipeD[2];
int PipeE[2];

void A () {
	close (PipeA[1]);
	close (PipeB[0]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[0]);
	close (PipeE[1]);
	
	while (true) {
		//A recibe un mensaje de E
		read(PipeA[0],&readMessage,1);
		printf("A");
		fflush(NULL);
		
		//A envia un mensaje a B
        writeMessage = 'X';
		write(PipeB[1],&writeMessage,1);
		fflush(NULL);
	}
}

void B () {
	close (PipeA[0]);
	close (PipeA[1]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[0]);
	close (PipeE[1]);
	
	while (true) {
		//B recibe un mensaje de A
		read(PipeB[0],&readMessage,1);
		printf("B");
		fflush(NULL);
		
		//B envia un mensaje a C
        writeMessage = 'X';
		write(PipeC[1],&writeMessage,1);
		fflush(NULL);
	}
}

void C () {
	close (PipeA[0]);
	close (PipeA[1]);
	close (PipeB[0]);
	close (PipeB[1]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	
	while (true) {
		//C recibe un mensaje de B
		read(PipeC[0],&readMessage,1);
		printf("C");
		fflush(NULL);
		
		//C envia un mensaje a D
        writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
		fflush(NULL);
	}
}

void D () {
	close (PipeA[0]);
	close (PipeA[1]);
	close (PipeB[0]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[1]);
	close (PipeE[0]);
	
	while (true) {
		//D recibe un mensaje de C
		read(PipeD[0],&readMessage,1);
		printf("D");
		fflush(NULL);
		
		//D envia un mensaje a E
        writeMessage = 'X';
		write(PipeE[1],&writeMessage,1);
		fflush(NULL);
	}
}

void E () {
	close (PipeA[0]);
	close (PipeB[0]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[1]);
	
	while (true) {
		//E recibe un mensaje de D
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
		
		//E envia un mensaje a A
        writeMessage = 'X';
		write(PipeA[1],&writeMessage,1);
		fflush(NULL);
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
	
	pipe (PipeA);
	pipe (PipeB);
	pipe (PipeC);
	pipe (PipeD);
	pipe (PipeE);
	
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
	
	close (PipeA[0]);
	close (PipeB[0]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[0]);
	close (PipeE[1]);
	//El Padre envia un mensaje a A para empezar el ciclo
	writeMessage = 'X';
	fflush (NULL);
    write(PipeA[1],&writeMessage,1);
	fflush(NULL);

	for (I = 1; I <= cantP; I++) {
        wait (NULL);
    }
	
	return 0;
}
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

//(AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE...

/*
	
	A Hace: Recibe Mensaje PipeABC; Imprime A; Envia Mensaje PipeD;
	
	B Hace: Recibe Mensaje PipeABC; Imprime B; Envia Mensaje PipeD;
	
	C Hace: Recibe Mensaje PipeABC; Imprime C; Envia Mensaje PipeD;
	
	D Hace: Recibe Mensaje PipeDAux; Recibe Mensaje PipeD; Imprime D; Envia Mensaje PipeE;
	
	E Hace: Recibe Mensaje PipeD; Envia Mensaje PipeDAux; Envia Mensaje PipeABC; Recibe Mensaje PipeE; Imprime E; Envia Mensaje PipeABC;
	
*/

//0 es para leer y 1 es para escribir

char writeMessage;
char readMessage;

int PipeABC[2];
int PipeD[2];
int PipeE[2];
int PipeDAux[2];

void A () {
	close (PipeABC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);
	
	while (true) {
		//A recibe un mensaje de E
		read(PipeABC[0],&readMessage,1);
		printf("A");
		fflush(NULL);
		
		//A envia un mensaje a D
		writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
		fflush(NULL);
	}
}

void B () {
	close (PipeABC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);

	while (true) {
		//B recibe un mensaje de E
		read(PipeABC[0],&readMessage,1);
		printf("B");
		fflush(NULL);
		
		//B envia un mensaje a D
		writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
		fflush(NULL);
	}
}

void C () {
	close (PipeABC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);

	while (true) {
		//C recibe un mensaje de E
		read(PipeABC[0],&readMessage,1);
		printf("C");
		fflush(NULL);
		
		//C envia un mensaje a D
		writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
		fflush(NULL);
	}
}

void D () {
	close (PipeABC[0]);
	close (PipeABC[1]);
	close (PipeD[1]);
	close (PipeE[0]);
	close (PipeDAux[1]);

	while (true) {
		//D recibe un mensaje de E
		read(PipeDAux[0],&readMessage,1);
		fflush(NULL);
		
		//D recibe un mensaje de (AoBoC)
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
	close (PipeABC[0]);
	close (PipeD[1]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	
	while (true) {
		//E intercepta el primer mensaje para D enviado por (AoBoC)
		read(PipeD[0],&readMessage,1);
		fflush(NULL);
		
		//E le avisa a D que puede recibir el proximo mensaje de (AoBoC)
		writeMessage = 'X';
		write(PipeDAux[1],&writeMessage,1);
		fflush(NULL);
		
		//E envia un mensaje a (AoBoC)
		writeMessage = 'X';
		write(PipeABC[1],&writeMessage,1);
		fflush(NULL);
		
		//E recibe un mensaje de D
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
		
		//E envia un mensaje a (AoBoC)
		writeMessage = 'X';
		write(PipeABC[1],&writeMessage,1);
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
	
	pipe (PipeABC);
	pipe (PipeD);
	pipe (PipeE);
	pipe (PipeDAux);
	
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
	
	close (PipeABC[0]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[1]);
	close (PipeE[0]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);
	//El Padre envia un mensaje a (AoBoC) para empezar el ciclo
	writeMessage = 'X';
	fflush (NULL);
	write(PipeABC[1],&writeMessage,1);
	fflush(NULL);
	
	for (I = 1; I <= cantP; I++) {
        wait (NULL);
    }
	
	return 0;
}
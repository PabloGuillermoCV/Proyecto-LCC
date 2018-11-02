#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

//(AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE...

/*
	
	A Hace: Recibe Mensaje PipeA; Imprime A; Envia Mensaje PipeD;
	
	B Hace: Recibe Mensaje PipeB; Imprime B; Envia Mensaje PipeD;
	
	C Hace: Recibe Mensaje PipeC; Imprime C; Envia Mensaje PipeD;
	
	D Hace: Recibe Mensaje PipeDAux; Recibe Mensaje PipeD; Imprime D; Envia Mensaje PipeE;
	
	E Hace: Recibe Mensaje PipeD; Envia Mensaje PipeDAux; Envia Mensaje Pipe(AoBoC); Recibe Mensaje PipeE; Envia Mensaje Pipe(AoBoC);
	
*/

//0 es para leer y 1 es para escribir

    char writeMessage;
	char readMessage;

	int PipeA[2];
	int PipeB[2];
    int PipeC[2];
	int PipeD[2];
	int PipeE[2];
	int PipeDAux[2];

void A () {
	close (PipeA[1]);
	close (PipeB[0]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);
	while (true) {
		read(PipeA[0],&readMessage,1);
		printf("A");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
	}
}

void B () {
	close (PipeA[0]);
	close (PipeA[1]);
	close (PipeB[1]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeE[0]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	close (PipeDAux[1]);
	while (true) {
		read(PipeB[0],&readMessage,1);
		printf("B");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
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
	close (PipeDAux[0]);
	close (PipeDAux[1]);
	while (true) {
		read(PipeC[0],&readMessage,1);
		printf("C");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeD[1],&writeMessage,1);
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
	close (PipeDAux[1]);
	while (true) {
		read(PipeDAux[0],&readMessage,1);
		fflush(NULL);
		read(PipeD[0],&readMessage,1);
		fflush(NULL);
		printf("D");
		writeMessage = 'X';
		write(PipeE[1],&writeMessage,1);
		fflush(NULL);
	}
}

void E () {
	close (PipeA[0]);
	close (PipeB[0]);
	close (PipeC[0]);
	close (PipeD[1]);
	close (PipeE[1]);
	close (PipeDAux[0]);
	
	int NumRandom;
	writeMessage = 'X';
	NumRandom = rand() % 3 + 1;
	switch (NumRandom) {
		case 1 : write(PipeA[1],&writeMessage,1); break;
		case 2 : write(PipeB[1],&writeMessage,1); break;
		case 3 : write(PipeC[1],&writeMessage,1); break;
	}
	fflush(NULL);
	
	while (true) {
		read(PipeD[0],&readMessage,1);
		fflush(NULL);
		writeMessage = 'X';
		write(PipeDAux[1],&writeMessage,1);
		fflush(NULL);
		
		NumRandom = rand() % 3 + 1;
		switch (NumRandom) {
			case 1 : write(PipeA[1],&writeMessage,1); break;
			case 2 : write(PipeB[1],&writeMessage,1); break;
			case 3 : write(PipeC[1],&writeMessage,1); break;
		}
		fflush(NULL);
		
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
		writeMessage = 'X';
		
		NumRandom = rand() % 3 + 1;
		switch (NumRandom) {
			case 1 : write(PipeA[1],&writeMessage,1); break;
			case 2 : write(PipeB[1],&writeMessage,1); break;
			case 3 : write(PipeC[1],&writeMessage,1); break;
		}
		fflush(NULL);
	}
}

int main () {
	
	pipe (PipeA);
	pipe (PipeB);
	pipe (PipeC);
	pipe (PipeD);
	pipe (PipeE);
	pipe (PipeDAux);
	
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

	if (pid != 0){
    	wait(NULL);
	}
	
	return 0;
}
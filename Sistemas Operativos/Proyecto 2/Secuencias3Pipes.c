#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

//ACDE BCDE ABCDE  ACDE BCDE ABCDE...

/*
	
	A Hace: Recibe Mensaje PipeA; Imprime A; Envia Mensaje PipeC; Recibe Mensaje PipeA; Imprime A; Envia Mensaje PipeB;
	
	B Hace: Recibe Mensaje PipeB; Imprime B; Envia Mensaje PipeC;
	
	C Hace: Recibe Mensaje PipeC; Imprime C; Envia Mensaje PipeD;
	
	D Hace: Recibe Mensaje PipeD; Imprime D; Envia Mensaje PipeE;
	
	E Hace: Recibe Mensaje PipeE; Imprime E; Envia Mensaje PipeB; Recibe Mensaje PipeE; Imprime E; Envia Mensaje PipeA; Recibe Mensaje PipeE; Imprime E; Envia Mensaje PipeA;
	
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
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[0]);
	close (PipeE[1]);
	while (true) {
		read(PipeA[0],&readMessage,1);
		printf("A");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeC[1],&writeMessage,1);
		
		read(PipeA[0],&readMessage,1);
		printf("A");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeB[1],&writeMessage,1);
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
		read(PipeB[0],&readMessage,1);
		printf("B");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeC[1],&writeMessage,1);
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
	while (true) {
		read(PipeD[0],&readMessage,1);
		printf("D");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeE[1],&writeMessage,1);
	}
}

void E () {
	close (PipeA[0]);
	close (PipeB[0]);
	close (PipeC[0]);
	close (PipeC[1]);
	close (PipeD[0]);
	close (PipeD[1]);
	close (PipeE[1]);
	writeMessage = 'X';
    write(PipeA[1],&writeMessage,1);
	while (true) {
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeB[1],&writeMessage,1);
		
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeA[1],&writeMessage,1);
		
		read(PipeE[0],&readMessage,1);
		printf("E");
		fflush(NULL);
        writeMessage = 'X';
		write(PipeA[1],&writeMessage,1);
	}
}

int main () {
	
	pipe (PipeA);
	pipe (PipeB);
	pipe (PipeC);
	pipe (PipeD);
	pipe (PipeE);
	
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
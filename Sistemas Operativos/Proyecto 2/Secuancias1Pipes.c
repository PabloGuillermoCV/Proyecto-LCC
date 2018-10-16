#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

//ABCDE ABCDE ABCDE ABCDE...

/*
	
	sem(A) = 1
		A hace: down(A), Imprimir, up(B)
	sem(B) = 0
		B hace: down(B), Imprimir, up(C)
	sem(C) = 0
		C hace: down(C), Imprimir, up(D) 
	sem(D) = 0
		D hace: down(D), Imprimir, up(E)
	sem(E) = 0
		E hace: down(E), Imprimir, up(A)
	
*/

void A () {
	
}

void B () {
	
}

void C () {
	
}

void D () {
	
}

void E () {
	
}

int main () {
	
	char writeMessage[20];
	char readMessage[20];
	
	int pdf [2];
	pipe (pdf);
	
    int pid = fork ();
    if (pid == -1) {
    	fprintf (stderr,"Error al crear el Proceso Hijo A");
    }
    else {
    	if (pid != 0) {
        	pid = fork ();
			if (pid == -1) {
				fprintf (stderr,"Error al crear el Proceso Hijo B");
			}
			else {
				if (pid == 0) A ();
				if (pid != 0) {
					pid = fork ();
					if (pid == -1) {
						fprintf (stderr,"Error al crear el Proceso Hijo C");
					}
					else {
						if (pid == 0) B ();
						if (pid != 0) {
							pid = fork ();
							if (pid == -1) {
								fprintf (stderr,"Error al crear el Proceso Hijo D");
							}
							else {
								if (pid == 0) C ();
								if (pid != 0) {
									pid = fork ();
									if (pid == -1) {
										fprintf (stderr,"Error al crear el Proceso Hijo E");
									}
									else {
										if (pid == 0) D ();
										if (pid != 0) {
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

	if (pid == 0){
    	wait(NULL);
	}
	
	
	
	return 0;
}
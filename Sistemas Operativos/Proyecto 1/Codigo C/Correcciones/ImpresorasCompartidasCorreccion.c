#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define N 6

/*
Ej sincronización (impresoras sin prioridad):

+) Compila sin warnings
+) Solo 2 usuarios pueden acceder a las impresoras a la vez

-) Mal sincronización. No se bloquea una impresora especifica.
-) No implementaron las funciones requerir y liberar
-) El usuario no indica que usuario es ni que impresora esta usando
*/

sem_t Impresora1;
sem_t Impresora2;

struct ID_Usuario {
	int ID;
}
struct ID_Usuario datosStruct [N];

void Requerir (sem_t I) {
	switch (I) {
		case 1: sem_wait (&Impresora1); break;
		case 2: sem_wait (&Impresora2); break;
	}
}

void Liberar (sem_t I) {
	switch (I) {
		case 1: sem_post (&Impresora1); break;
		case 2: sem_post (&Impresora2); break;
	}
}

void *UsuarioSolicita (void *threadarg) {
	
	struct ID_Usuario *U;
	U = (struct ID_Usuario *) threadarg;
	
	int Imp;
    int C;
	for (C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		Imp = rand () % 3;
		Requerir (Imp);
		printf ("El Usuario %d Requirio La Impresora %d.\n",U->ID,Imp);
		sleep (4);
		printf ("El Usuario %d Libero La Impresora %d.\n",U->ID,Imp);
		Liberar (Imp);
	}
	exit (0);
}

int main () {
	
	sem_init (&Impresora1,0,1);
	sem_init (&Impresora2,0,1);
	
	pthread_t Usuarios [N];
	
	int I;
	
	for (I = 0; I < N; I++) {
		datosStruct[I].ID = I;
		rc = pthread_create(&Usuarios[I], NULL, UsuarioSolicita, (void *) datosStruct[I]);
		if (rc){ //Ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}

	for (I = 0; I < N; I++){
		pthread_join (Usuarios[I], NULL);
	}
	
	sem_destroy (&Impresora1);
	sem_destroy (&Impresora2);
	
	printf ("Terminaron Todas Las Impresiones.\n");
	
	return 0;
}
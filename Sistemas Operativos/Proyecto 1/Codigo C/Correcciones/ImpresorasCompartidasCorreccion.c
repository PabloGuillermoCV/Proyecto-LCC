#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define N 6 //Numero de usuarios
#define Imp1 1 //ID de la impresora 1
#define Imp2 2 //ID de la impresora 2

/*
Correcciones Ej sincronización (impresoras sin prioridad):

+) Compila sin warnings
+) Solo 2 usuarios pueden acceder a las impresoras a la vez

-) Mal sincronización. No se bloquea una impresora especifica.
-) No implementaron las funciones requerir y liberar
-) El usuario no indica que usuario es ni que impresora esta usando
*/

/*Algoritmo Impresoras Sin Prioridad

	//UsuarioSolicita:
	//Durante 20 ciclos, hacer
		//Requerir
		//Sleep 3 Segundos
		//Liberar
		//Sleep 1 Segundo
	
	//Requerir:
	//Wait (PermisoLibre)
	//Trywait (Impresora1)
		//Retornar Impresora 1
	//Sino
		//Trywait (Impresora2)
			//Retornar Impresora 2
		//Sino
			//Se produjo un error
	
	//Liberar:
	//Liberar impresora pasada por parametro (signal)
	//Signal (PermisoLibre)
*/

sem_t PermisoLibre; //Semaforo que indica cuantas impresoras estan libres
sem_t Impresora1; //Semaforo para la impresora 1
sem_t Impresora2; //Semaforo para la impresora 2

struct ID_Usuario {
	int ID;
};
struct ID_Usuario datosStruct [N];

int Requerir () {
	int S;
	sem_wait (&PermisoLibre);
	S = sem_trywait (&Impresora1);
	if (S == 0) {
		return Imp1;
	}
	else {
		S = sem_trywait (&Impresora2);
		if (S == 0) {
			return Imp2;
		}
		else {
			printf ("Ocurrio Un Error En La Sincronizacion.\n");
			exit (1);
		}
	}
}

void Liberar (int I) {
	switch (I) {
		case 1: sem_post (&Impresora1); break;
		case 2: sem_post (&Impresora2); break;
	}
	sem_post (&PermisoLibre);
}

void *UsuarioSolicita (void *threadarg) {
	
	struct ID_Usuario *U;
	U = (struct ID_Usuario *) threadarg;
	
	int Id = U->ID;
	
	int Imp;
    int Ciclo;
	
	for (Ciclo = 0; Ciclo < 20; Ciclo++) { //Este usuario solicitara durante 20 ciclos
		Imp = Requerir ();
		printf ("El Usuario %d Requirio La Impresora %d.\n",Id,Imp);
		sleep (3); //Tiempo que tarda antes de liberarla
		printf ("El Usuario %d Libero La Impresora %d.\n",Id,Imp);
		Liberar (Imp);
		sleep (1); //Esperan un poco antes de volver a requerir
	}
	exit (0);
}

int main () {
	
	sem_init (&PermisoLibre,0,2);
	sem_init (&Impresora1,0,1);
	sem_init (&Impresora2,0,1);
	
	pthread_t Usuarios [N];
	
	int I;
	int rc;
	
	for (I = 0; I < N; I++) {
		datosStruct[I].ID = I;
		rc = pthread_create (&Usuarios[I], NULL, UsuarioSolicita, (void *) &datosStruct[I]);
		if (rc) { //Ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit (-1);
       	}
	}

	for (I = 0; I < N; I++) {
		pthread_join (Usuarios[I], NULL);
	}
	
	sem_destroy (&PermisoLibre);
	sem_destroy (&Impresora1);
	sem_destroy (&Impresora2);
	
	printf ("Terminaron Todas Las Impresiones.\n");
	
	return 0;
}
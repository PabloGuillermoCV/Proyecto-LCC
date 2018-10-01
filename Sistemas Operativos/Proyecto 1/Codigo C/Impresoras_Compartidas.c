#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_THREADS 6

/*Algoritmo RequerirEImprimir

	//Durante 20 ciclos, hacer
		//Entry
			//Requerir(ImpresorasDisponibles)
			//Mientras no tengo el lock, esperar
		//Sección Crítica, obtuve el lock
			//Hacer trabajo de impresion
		//Exit
			//Liberar(ImpresorasDisponibles)

*/

//Semaforo para las dos impresoras
sem_t impresorasDisponibles;

//Funcion que usa un usuario para solicitar una impresora
void *RequerirEImprimir () {

    int C;
	for (C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		sem_wait(&impresorasDisponibles); //El usuario solicita una impresora
		printf ("Impriendo...\n");
		sleep (4);
		printf ("Fin De La Impresion.\n");
		sem_post(&impresorasDisponibles); //El usuario deja de usar una impresora
	}
	exit (1);
}

int main(){

	sem_init(&impresorasDisponibles,0,2);

	pthread_t Hilos [NUM_THREADS];

	int rc;
	int I;
	for (I = 0; I < NUM_THREADS; I++){
		rc = pthread_create(&Hilos[I], NULL, RequerirEImprimir, NULL);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}

	for(I = 0; I < NUM_THREADS; I++){
		pthread_join(Hilos[I], NULL);
	}

	sem_destroy(&impresorasDisponibles);

	printf("Terminaron Todas Las Impresiones.\n");

	return 0;
}

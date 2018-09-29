#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
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

	for (int C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		sem_wait(&impresorasDisponibles); //El usuario solicita una impresora
		printf ("Impriendo...");
		sleep (4);
		printf ("Fin De La Impresion.");
		sem_post(&impresorasDisponibles); //El usuario deja de usar una impresora
	}
	exit (1);
}

int main(){
	
	sem_init(&impresorasDisponibles,0,2);

	pthread_t Hilos [NUM_THREADS];

	int rc;
	for (int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&Hilos[i], NULL, RequerirEImprimir, NULL);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}
	
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(Hilos[i], NULL);
	}

	sem_destroy(&impresorasDisponibles);
	
	return 0;
}
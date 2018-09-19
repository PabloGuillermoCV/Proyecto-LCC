#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

#define NUM_Threads 6

/*Algoritmo

	//Entry
		id = requerir(impresora)
		mientras no tengo el lock, esperar
	//Sección Crítica, obtuve el lock
		//hacer trabajo
	//Exit
		liberar(id)

*/

sem_t impresoras;


void *Imprimir(void *threadarg){

	//Esperar
	sem_wait(&impresoras);
	printf("\n entrando a SC...\n");

	//Sección Crítica
	//esto es para debuggear, al terminar se saca
	sleep(4);

	//liberar
	printf("\n Saliendo de la SC \n");
	sem_post(&impresoras);


}

//Variable Global para obtener los Argumentos para cada Thread
struct datos_thread thread_Data[NUM_THREADS];

int main(){
	
	sem_init(&impresoras, 0, 2);

	pthread_t Hilos[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&Hilos[j], NULL, Imprimir, NULL);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
                             	
	}

	for(int j ){

		pthread_join(Hilos[i], NULL);
	}
	sem_destroy(&impresoras);
	return 0;
}
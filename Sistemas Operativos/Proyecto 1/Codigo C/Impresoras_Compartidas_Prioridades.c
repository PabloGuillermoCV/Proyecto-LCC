#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

#define NUM_THREADS 6
/*Algoritmo
	
	//Entry
		id = Requerir(Impresora)
		mientras no tengo el lock{
			prioridad++ //Evito Starvation
			dormir 1 segund
		}
	//Sección Critica, obtuve el lock
		Trabajar
	//Exit
		liberar(id);
*/


void *Imprimir(void *threadarg){

	struct datos_thread *misDatos;
	misDatos = (struct thread_data *) threadarg; //obtengo la estructura de argumentos que posee el Thread

}

struct datos_hilo{
	int Thread_id;
	int prioridad;
}

//Variable Global para obtener los Argumentos para cada Thread
struct datos_thread thread_Data[NUM_THREADS];

int main(){
	
	pthread_t Hilos[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&Hilos[j], NULL, Imprimir, (void *) &thread_Data[i]);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}
	
}


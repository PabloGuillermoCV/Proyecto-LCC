#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

#define NUM_THREADS 6

/*Algoritmo

	//Entry
		id = requerir(impresora)
		mientras no tengo el lock, esperar
	//Sección Crítica, obtuve el lock
		//hacer trabajo
	//Exit
		liberar(id)

*/

//Semaforo para las dos impresoras
sem_t impresorasDisponibles;

//Semaforos que indican si un hilo esta esperando o no a que la impresora termine su trabajo
sem_t esperandoImpresion [NUM_THREADS];

struct datos_hilo {
	int Thread_id;
	int orden;
};

//Variable Global para obtener los Argumentos para cada Thread
struct datos_hilo thread_Data[NUM_THREADS];

//Funcion que usa una impresora para responder a los pedidos de un usario
void Imprimir() {
	for (int C = 0; C < 60; C++) { //Esta impresora trabajara durante 60 ciclos
		
	}
	exit (1);
}

//Funcion que usa un usuario para solicitar una impresora
void *Requerir(void *threadarg){

	struct datos_thread *misDatos;
	//no son necesarios las impresoras hilos, es simplemente que entre aca, haga la zona critica aca y salga para que le deje el paso a otro usuario, es un proceso normal, solo tengo esto
	misDatos = (struct thread_data *) threadarg; //Obtengo la estructura de argumentos que posee el Thread

	for (int C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		sem_wait(&impresorasDisponibles); //El usuario solicita una impresora
		sem_wait(&esperandoImpresion[misDatos.orden]);
		sleep (4);
	}
	exit (1);
}

int main(){
	
	sem_init(&impresorasDisponibles,0,2);

	pthread_t Hilos[NUM_THREADS];

	pthread_t Impresora1;
	pthread_t Impresora2;

	int rc;

	rc = pthread_create(Impresora1,NULL,Imprimir,NULL);
	if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
    }
	rc = pthread_create(Impresora2,NULL,Imprimir,NULL);
	if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
    }    

	for(int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&Hilos[i], NULL, Imprimir, NULL);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(Hilos[i], NULL);
	}
	pthread_join(Impresora1,NULL);
	pthread_join(Impresora2,NULL);

	sem_destroy(&impresorasDisponibles);
	return 0;
}
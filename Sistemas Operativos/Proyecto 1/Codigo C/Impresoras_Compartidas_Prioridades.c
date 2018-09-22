#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

#define NUM_THREADS 6

/*Algoritmo
	
	//Entry
		Requerir(Prioridades[prioridad propia])
		Requerir(Impresora)
		mientras no tengo el lock{
			dormir 1 segund
		}
	//Sección Critica, obtuve el lock
		Trabajar
	//Exit
		liberar(id);
		liberar a la prioridad bloqueada mas alta
*/

//Semaforo para las dos impresoras
sem_t impresoras;

//Semaforos que le permiten o bloquean el paso a cada hilo
sem_t prioridades [NUM_THREADS];

struct datos_hilo{
	int Thread_id;
	int prioridad;
};

//Variable Global para obtener los Argumentos para cada Thread
struct datos_hilo thread_Data[NUM_THREADS];

//Busca la prioridad mas alta bloqueada y le permite el paso
void liberarMasAlta(sem_t *P []) {
	bool Encontre = false;
	int Valor;
	for (int i = NUM_THREADS-1; i > -1 && !Encontre; i--) {
		sem_getvalue(&P[i],Valor);
		if (Valor == 0) {
			sem_post(&P[i]);
			Encontre = true;
		}
	}
}

void *Imprimir(void *threadarg){
	
	struct datos_thread *misDatos;
	misDatos = (struct thread_data *) threadarg; //obtengo la estructura de argumentos que posee el Thread
	
	//Esperar
	sem_wait(&prioridades[misDatos.prioridad]);
	sem_wait(&impresoras);
	printf("\n entrando a SC...\n");

	//Sección Crítica
	//esto es para debuggear, al terminar se saca
	sleep(4);

	//liberar
	printf("\n Saliendo de la SC \n");
	sem_post(&impresoras);
	liberarMasAlta(&prioridades);
}

int main(){
	
	sem_init(&impresoras, 0, 2);
	
	pthread_t Hilos[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++){
		thread_Data[i].prioridad = i;
		sem_init(&prioridades[i],0,1);
		rc = pthread_create(&Hilos[j], NULL, Imprimir, (void *) &thread_Data[i]);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}
	for(int j = 0; j < NUM_THREADS; j++){
		pthread_join(Hilos[i], NULL);
	}
	sem_destroy(&impresoras);
	return 0;
}
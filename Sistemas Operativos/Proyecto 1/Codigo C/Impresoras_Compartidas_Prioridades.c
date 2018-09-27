#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

#define NUM_THREADS 6

/*Algoritmo Requerir
	
	//Durante 20 ciclos, hacer
		//Liberar(Prioridades propia) para entrar en la lista de pedidos
		//Requerir(EsperandoImpresion propia) para esperar a que termine de imprimir
*/

/*Algoritmo Imprimir

	//Durante 60 ciclos, hacer
		//Requerir(BloquearImpresora) para evitar que ambas impresoras se sobrepongan
		//Encontre<-false
		//Para cada usuario, comenzando desde la prioridad mas alta, se recupera el primer pedido disponible encontrado y mientras Encontre=false
			//Intentar Requerir(Prioridades)
				//Si se puede, Liberar(BloquearImpresora) para que la otra busque un pedido
				//Realizar trabajo impresion
				//Liberar(EsperandoImpresion) para que el usuario continue su trabajo
				//Encontre<-true
		//Si no Encontre pedido disponible
			//Liberar(BloquearImpresora) para que la otra impresora realize su busqueda
*/

//Semaforos que le permiten o bloquean el paso a cada hilo
sem_t prioridades [NUM_THREADS];

//Semaforos que indican si un hilo esta esperando o no a que la impresora termine su trabajo
sem_t esperandoImpresion [NUM_THREADS];

//Semaforo que se encarga de bloquearle el paso a una impresora cuando la otra esta trabajando
sem_t bloquearImpresora;

struct datos_hilo {
	int Thread_id;
	int prioridad;
};

//Variable Global para obtener los Argumentos para cada Thread
struct datos_hilo thread_Data [NUM_THREADS];

//Funcion que usa una impresora para responder a los pedidos de un usario
void *Imprimir() {
	bool Encontre;
	for (int C = 0; C < 60; C++) { //Esta impresora trabajara durante 60 ciclos
		sem_wait(&bloquearImpresora);
		Encontre = false;
		for (int I = NUM_THREADS - 1; I > -1 && !Encontre; I--) {
			sem_trywait(&prioridades[I]) { //Verifica si este usuario necesita la impresora
				sem_post(&bloquearImpresora);
				printf ("Imprimiendo Para El Usuario.");
				sem_post(&esperandoImpresion[I]); //Le aviso al usuario que la impresora termino de imprimir
				Encontre = true;
			}
		}
		if (Encontre == false) {
			sem_post(&bloquearImpresora); //En caso de no encontrar ningun pedido, le deja intentar a la otra impresora
		}
	}
	exit (1);
}

//Funcion que usa un usuario para solicitar una impresora
void *Requerir(void *threadarg){
	
	struct datos_thread *misDatos;
	misDatos = (struct thread_data *) threadarg; //Obtengo la estructura de argumentos que posee el Thread
	
	for (int C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		sem_post(&prioridades[misDatos.prioridad]); //Entra en la cola para solicitar una impresora
		sem_wait(&esperandoImpresion[misDatos.prioridad]); //Espera a que la impresora termine de imprimir
	}
	exit (1);
}

int main(){

	sem_init(&bloquearImpresora,0,1);

	pthread_t Hilos [NUM_THREADS];

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
		thread_Data[i].prioridad = i;
		sem_init(&prioridades[i],0,0);
		sem_init(&esperandoImpresion[i],0,0);

		rc = pthread_create(&Hilos[j], NULL, Requerir, (void *) &thread_Data[i]);
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

	sem_destroy(&bloquearImpresora);
	for (int i = 0; i < NUM_THREADS; i++) {
		sem_destroy(&prioridades);
		sem_destroy(&esperandoImpresion);
	}
	
	return 0;
}
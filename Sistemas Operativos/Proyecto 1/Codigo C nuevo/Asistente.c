#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_THREADS 6

/*Algoritmo Atender

	//Durante 60 ciclos, hacer
		//Esperar un turno de un alumno (wait(EsperarAlumno))
		//Atender alumno
		//Liberar(Atendido)
*/

/*Algoritmo Solicitar

	//Durante 20 ciclos, hacer
		//Verificar si hay asientos (trywait(Asiento))
		//Si no hay Asientos
			//Irse y esperar un tiempo antes de volver a intentar
		//Si hay un asiento disponible, ocuparlo
			//Pedir turno (liberar(EsperarTurno))
			//El alumno espera a que la oficina este libre para entrar, no deja su asiento todavia (wait(OficinaLibre))
			//Liberar(Asiento)
			//Esperar a que termine de ser atentido por el asistente (wait(Atendido))
			//El alumno abandona la oficina (Liberar(OficinaLibre))
			//Al terminar, esperar un tiempo antes de volver a consultar
*/

//Semaforo usado para indicarle al asistente que hay alumnos esperando su turno
sem_t esperarTurno;

//Semaforo usado para indicar que un alumno debe esperar a que termine de ser atendido para seguir (binario)
sem_t atendido;

//Semaforo que nota la cantidad de asientos disponibles (hasta 3)
sem_t asiento;

//Semaforo que sirve para notificar cuando esta ocupada la oficina
sem_t oficinaLibre;

void *Atender () {

	int C;
	for (C = 0; C < 60; C++) { //El asistente atendera durante 60 ciclos
		sem_wait (&esperarTurno); //El asistente espera hasta que alguien espere por su turno
		sleep (4); //Tiempo que tarda el asistente en atender a un alumno
		printf ("Se atiende a un alumno.\n");
		sem_post (&atendido); //El asistente le avisa al alumno que termino
	}
	exit (1);
}

void *Solicitar () {

    int C;
	for (C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
        int A = sem_trywait(&asiento);
        if (A > -1) { //Si hay asientos disponibles espera su turno, sino se va
            printf("Se ocupa un asiento.\n");
			sem_post (&esperarTurno); //El alumno llega y pide el turno
			sem_wait (&oficinaLibre); //El alumno espera a que le dejen pasar
			printf("Se pasa a la oficina y se libera un asiento.\n");
			sem_post (&asiento); //Atienden al alumno y deja libre el asiento para otro alumno
			sem_wait (&atendido); //El alumno pasa de aca cuando el asistente termine
			sem_post (&oficinaLibre); //El alumno deja libre la oficina para alguno de los que esta esperando
		}
		sleep (10); //El alumno espera antes de volver al asistente
	}
	exit (1);
}

int main(){

	sem_init (&esperarTurno,0,0);
	sem_init (&atendido,0,0);
	sem_init (&asiento,0,3);
	sem_init (&oficinaLibre,0,1);

	pthread_t Alumnos [NUM_THREADS];

	pthread_t Asistente;

	int rc;

	rc = pthread_create(&Asistente, NULL, Atender, NULL);
	if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}

    int I;
	for(I = 0; I < NUM_THREADS; I++){
		rc = pthread_create(&Alumnos[I], NULL, Solicitar, NULL);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}

	for(I = 0; I < NUM_THREADS; I++){
		pthread_join (Alumnos[I], NULL);
	}
	pthread_join (Asistente,NULL);

	sem_destroy (&esperarTurno);
	sem_destroy (&atendido);
	sem_destroy (&asiento);
	sem_destroy (&oficinaLibre);

	printf("Terminaron Las Consultas.\n");

	return 0;
}

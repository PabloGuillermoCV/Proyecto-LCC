#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define N 6 //Numero de alumnos

/*
Correcciones Asistente (diseño e implementación):

+) Si alumno no puede sentarse se va y vuelve luego de un tiempo

-) Mal sincronización entre alumno y asistente
-) Asistente atiende de a 3 a la vez
*/

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

sem_t EsperarTurno; //Semaforo usado para indicarle al asistente que hay alumnos esperando su turno
sem_t Atendido; //Semaforo usado para indicar que un alumno debe esperar a que termine de ser atendido para seguir (binario)
sem_t Asiento; //Semaforo que nota la cantidad de asientos disponibles (hasta 3)
sem_t OficinaLibre; //Semaforo que sirve para notificar cuando esta ocupada la oficina

struct ID_Alumno {
	int ID;
};
struct ID_Alumno datosStruct [N];

void *Atender () {
	int Ciclo;
	for (Ciclo = 0; Ciclo < 60; Ciclo++) { //El asistente atendera durante 60 ciclos
		sem_wait (&EsperarTurno); //El asistente espera hasta que alguien espere por su turno
		sleep (4); //Tiempo que tarda el asistente en atender a un alumno
		printf ("Se atiende a un alumno.\n");
		sem_post (&Atendido); //El asistente le avisa al alumno que termino
	}
	exit (0);
}

void *Solicitar (void *threadarg) {
	struct ID_Alumno *A;
	A = (struct ID_Alumno *) threadarg;
	
	int Id = A->ID;
	
    int Ciclo;
	for (Ciclo = 0; Ciclo < 20; Ciclo++) { //Este usuario solicitara durante 20 ciclos
        int A = sem_trywait (&Asiento);
        if (A == 0) { //Si hay asientos disponibles espera su turno, sino se va
            printf ("El alumno %d ocupa un asiento.\n",Id);
			sem_post (&EsperarTurno); //El alumno llega y pide el turno
			sem_wait (&OficinaLibre); //El alumno espera a que le dejen pasar
			printf ("El alumno %d pasa a la oficina y se libera un asiento.\n",Id);
			sem_post (&Asiento); //Atienden al alumno y deja libre el asiento para otro alumno
			sem_wait (&Atendido); //El alumno pasa de aca cuando el asistente termine
			sem_post (&OficinaLibre); //El alumno deja libre la oficina para alguno de los que esta esperando
		}
		sleep (10); //El alumno espera antes de volver al asistente
	}
	exit (0);
}

int main () {
	sem_init (&EsperarTurno,0,0);
	sem_init (&Atendido,0,0);
	sem_init (&Asiento,0,3);
	sem_init (&OficinaLibre,0,1);

	pthread_t Alumnos [N];

	pthread_t Asistente;
	
	int I;
	int rc;

	rc = pthread_create (&Asistente, NULL, Atender, NULL);
	if (rc) { //ocurrió un error al crear el Thread, reportar
		printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }

	for (I = 0; I < N; I++) {
		datosStruct[I].ID = I;
		rc = pthread_create(&Alumnos[I], NULL, Solicitar, (void *) &datosStruct[I]);
		if (rc) { //ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit (-1);
       	}
	}

	for (I = 0; I < N; I++){
		pthread_join (Alumnos[I], NULL);
	}
	pthread_join (Asistente,NULL);

	sem_destroy (&EsperarTurno);
	sem_destroy (&Atendido);
	sem_destroy (&Asiento);
	sem_destroy (&OficinaLibre);

	printf("Terminaron Las Consultas.\n");

	return 0;
}
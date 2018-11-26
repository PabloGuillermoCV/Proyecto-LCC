#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdbool.h>

#define N 6 //Numero de alumnos

/*
Correcciones Asistente (diseño e implementación):

+) Si alumno no puede sentarse se va y vuelve luego de un tiempo

-) Mal sincronización entre alumno y asistente
-) Asistente atiende de a 3 a la vez
*/

/*Algoritmos Asistente
	
	//Atender:
	While True, hacer
		Ver si hay alumnos esperando (Trywait(AlumnosEsperando))
			Avisar que esta listo para responder (Signal(EsperarTurno))
			Esperar a que el alumno este listo para responder (Wait(Consultar))
			Atender al alumno, la consulta tarda 4 segundos
			Avisar al alumno que la consulta termino (Signal(Atendido))
		Sino
			Dormir hasta que un alumno lo despierte (Wait(Dormir))
	
	//Solicitar:
	While True, hacer
	Verificar si hay asientos libres (Trywait(Asiento))
	Si hay un asiento disponible, se ocupa
		El alumno avisa que hay 1 mas esperando (Signal(AlumnosEsperando))
		El alumno se queda esperando a que la oficina este libre (Wait(OficinaLibre))
		El alumno se levanta, pasa a la oficina y libera el asiento (Post(Asiento))
		El alumno intenta ver si el asistente esta despierto (Trywait(EsperarTurno))
		Si esta dormido
			Lo despierta (Signal(Dormir))
			Espera a que el asistente este bien despierto (Wait(EsperarTurno))
		El alumno le dice al asistente que esta listo para consultar (Signal(Consultar))
		El alumno espera a que termine la consulta (Wait(Atendido))
		El alumno abandona la oficina y se va (Signal(OficinaLibre))
		Se va y no vuelve a consultar por 10 segundos
	Sino
		El alumno se va y vuelve a intentar en 10 segundos
*/

sem_t EsperarTurno; //Semaforo usado para indicarle al alumno que ya esta listo. Empieza en 0.
sem_t Atendido; //Semaforo usado para indicar que un alumno debe esperar a que termine de ser atendido para seguir (binario). Empieza en 0.
sem_t Asiento; //Semaforo que nota la cantidad de asientos disponibles (hasta 3). Empieza en 3.
sem_t OficinaLibre; //Semaforo que sirve para notificar cuando esta ocupada la oficina. Empieza en 1.
sem_t AlumnosEsperando; //Semaforo que cuenta cuantos alumnos hay esperando turno, sirve para saber si el asistente duerme o no. Empieza en 0.
sem_t Dormir; //Semaforo mutex que sirve para que el asistente duerma y sea despertado por un alumno. Empieza en 0.
sem_t Consultar; //Semaforo mutex que sirve para que el alumno comienze a consultarle las dudas al asistente. Empieza en 0.

struct ID_Alumno {
	int ID;
};
struct ID_Alumno datosStruct [N]; //Estructura usada para definir la ID de un alumno.

void *Atender () {
	int S;
	while (true) {
		S = sem_trywait (&AlumnosEsperando); //El asistente mira si hay alumnos esperando
		if (S == 0) {
			sem_post (&EsperarTurno); //El asistente esta despierto y listo para responder dudas
			sem_wait (&Consultar); //El asistente espera hasta que alguien espere por su turno
			printf ("El asistente comienza a atender al alumno.\n");
			sleep (4); //Tiempo que tarda el asistente en atender a un alumno
			printf ("El asistente termino de atender un alumno.\n");
			sem_post (&Atendido); //El asistente le avisa al alumno que termino
		}
		else { //Si no hay alumnos esperando, se duerme
			printf ("El asistente duerme.\n");
			sem_wait (&Dormir); //El asistente se duerme hasta que lo despierten
		}
	}
	exit (0);
}

void *Solicitar (void *threadarg) {
	struct ID_Alumno *A;
	A = (struct ID_Alumno *) threadarg;
	
	int Id = A->ID;
	
	int S;
	
	while (true) {
        int A = sem_trywait (&Asiento);
        if (A == 0) { //Si hay asientos disponibles espera su turno, sino se va
			sem_post (&AlumnosEsperando); //Hay un alumno mas esperando
            printf ("El alumno %d ocupa un asiento.\n",Id);
			sem_wait (&OficinaLibre); //El alumno espera a que le dejen pasar
			printf ("El alumno %d pasa a la oficina y se libera un asiento.\n",Id);
			sem_post (&Asiento); //Atienden al alumno y deja libre el asiento para otro alumno
			S = sem_trywait (&EsperarTurno); //El alumno mira si el asistente va a ayudarlo
			if (S != 0) { //Si ve que esta dormido, lo despierta
				printf ("El alumno %d despierta al asistente.\n",Id);
				sem_post (&Dormir); //El alumno despierta al asistente
				sem_wait (&EsperarTurno); //El alumno espera a que se despierte
			}
			printf ("El alumno %d va a consultarle al asistente despierto.\n",Id);
			sem_post (&Consultar); //En este momento el alumno empieza a consultar dudas hasta que termina el tiempo
			sem_wait (&Atendido); //El alumno pasa de aca cuando el asistente termine
			printf ("El alumno %d fue atendido.\n",Id);
			printf ("El alumno %d abandono la oficina.\n",Id);
			sem_post (&OficinaLibre); //El alumno deja libre la oficina para alguno de los que esta esperando
		}
		sleep (10); //El alumno espera antes de volver al asistente
	}
	exit (0);
}

int main () {
	
	int I;
	int rc;
	
	sem_init (&EsperarTurno,0,0);
	sem_init (&Atendido,0,0);
	sem_init (&Asiento,0,3);
	sem_init (&OficinaLibre,0,1);
	sem_init (&AlumnosEsperando,0,0);
	sem_init (&Dormir,0,0);
	sem_init (&Consultar,0,0);
	
	pthread_t Alumnos [N];
	
	pthread_t Asistente;
	
	//Crea el hilo Asistente
	rc = pthread_create (&Asistente, NULL, Atender, NULL);
	if (rc) { //ocurrió un error al crear el Thread, reportar
		printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }
	
	//Crea los N hilos alumno
	for (I = 0; I < N; I++) {
		datosStruct[I].ID = I;
		rc = pthread_create(&Alumnos[I], NULL, Solicitar, (void *) &datosStruct[I]);
		if (rc) { //ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit (-1);
       	}
	}
	
	pthread_join (Asistente,NULL);
	for (I = 0; I < N; I++){
		pthread_join (Alumnos[I], NULL);
	}
	
	sem_destroy (&EsperarTurno);
	sem_destroy (&Atendido);
	sem_destroy (&Asiento);
	sem_destroy (&OficinaLibre);
	sem_destroy (&AlumnosEsperando);
	sem_destroy (&Dormir);
	sem_destroy (&Consultar);
	
	printf("Terminaron Las Consultas.\n");
	
	return 0;
}
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdbool.h>

#define N 6 //Numero de alumnos

//EXPLICAR QUE NO SE MODELA LO DE QUE EL ASISTENTE DUERME
//
//
//
//

/*
Correcciones Asistente (diseño e implementación):

+) Si alumno no puede sentarse se va y vuelve luego de un tiempo

-) Mal sincronización entre alumno y asistente
-) Asistente atiende de a 3 a la vez
*/

/*Algoritmos Asistente
	
	//Atender:
	While True, hacer
		Esperar hasta que alguien pida turno (Wait(EsperarTurno))
		Atender al alumno, la consulta tarda 4 segundos
		Avisar al alumno que la consulta termino (Signal(Atendido))
	
	//Solicitar:
	While True, hacer
	Verificar si hay asientos libres (Trywait(Asiento))
	Si hay un asiento disponible, se ocupa
		El alumno se sienta y avisa que esta esperando (Signal(EsperarTurno))
		El alumno se queda esperando a que la oficina este libre (Wait(OficinaLibre))
		El alumno se levanta, pasa a la oficina y libera el asiento (Post(Asiento))
		El alumno espera a que termine la consulta (Wait(Atendido))
		El alumno abandona la oficina y se va (Signal(OficinaLibre))
		Se va y no vuelve a consultar por 10 segundos
	Sino
		El alumno se va y vuelve a intentar en 10 segundos
*/

sem_t EsperarTurno; //Semaforo usado para indicarle al asistente que hay alumnos esperando su turno. Empieza en 0.
sem_t Atendido; //Semaforo usado para indicar que un alumno debe esperar a que termine de ser atendido para seguir (binario). Empieza en 0.
sem_t Asiento; //Semaforo que nota la cantidad de asientos disponibles (hasta 3). Empieza en 3.
sem_t OficinaLibre; //Semaforo que sirve para notificar cuando esta ocupada la oficina. Empieza en 1.

struct ID_Alumno {
	int ID;
};
struct ID_Alumno datosStruct [N]; //Estructura usada para definir la ID de un alumno.

void *Atender () {
	while (true) {
		sem_wait (&EsperarTurno); //El asistente espera hasta que alguien espere por su turno
		sleep (4); //Tiempo que tarda el asistente en atender a un alumno
		printf ("El asistente termino de atender un alumno.\n");
		sem_post (&Atendido); //El asistente le avisa al alumno que termino
	}
	exit (0);
}

void *Solicitar (void *threadarg) {
	struct ID_Alumno *A;
	A = (struct ID_Alumno *) threadarg;
	
	int Id = A->ID;
	
	while (true) {
        int A = sem_trywait (&Asiento);
        if (A == 0) { //Si hay asientos disponibles espera su turno, sino se va
            printf ("El alumno %d ocupa un asiento.\n",Id);
			sem_post (&EsperarTurno); //El alumno llega y pide el turno
			sem_wait (&OficinaLibre); //El alumno espera a que le dejen pasar
			printf ("El alumno %d pasa a la oficina y se libera un asiento.\n",Id);
			sem_post (&Asiento); //Atienden al alumno y deja libre el asiento para otro alumno
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
	
	printf("Terminaron Las Consultas.\n");
	
	return 0;
}
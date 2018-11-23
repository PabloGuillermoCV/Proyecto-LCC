#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdbool.h>

#define N 6 //Numero de usuarios
#define Imp1 1 //ID de la impresora 1
#define Imp2 2 //ID de la impresora 2

/*
Correcciones Impresoras con prioridad:

+) Se implementan impresoras como hilos
+) Se implementa la prioridad

-) Mal sincronización
-) No se implementa la función liberar, ni se modela
-) Compila con warnings
-) Cast a tipo inexistente
-) No ejecuta. Termina con error sin imprimir nada.
*/

sem_t Impresora1; //Semaforo para la impresora 1
sem_t Impresora2; //Semaforo para la impresora 2
sem_t BloquearImpresora; //Semaforo que hace que la busqueda de prioridad se haga de a 1
sem_t ColaPrioridades [N]; //Semaforos que indican cuando un usuario esta pidiendo impresora, ordenado por prioridad
sem_t EsperandoImpresion [N]; //Semaforos que indican que un usuario esta esperando a ser elegido

struct ID_Usuario {
	int ID;
	int Prioridad;
};
struct ID_Usuario datosStruct [N];

void *Imprimir1 () { //Metodo para la impresora 1
	bool Encontre;
	int Ciclo;
	int I;
	int S;
	
	for (Ciclo = 0; Ciclo < 60; Ciclo++) { //Esta impresora trabajara durante 60 ciclos
		sem_wait (&BloquearImpresora);
		Encontre = false;
		
		for (I = N - 1; I > -1 && !Encontre; I--) {
            S = sem_trywait (&ColaPrioridades[I]);
			
            if (S == 0) { //Si el usuario necesita la impresora
				//Le digo al usuario a cual entrar
				sem_post (&Impresora1);
				 //Le aviso al usuario que la impresora termino de imprimir
				sem_post (&EsperandoImpresion[I]);
				//Dejo que la otra impresora entre
				sem_post (&BloquearImpresora);
				Encontre = true;
				//Espero a que el usuario termine de usar la impresora
				sem_wait (&Impresora1);
			}
		}
		
		if (Encontre == false) {
			//En caso de no encontrar ningun pedido, le deja intentar a la otra impresora
			sem_post (&BloquearImpresora);
		}
	}
	exit (0);
}

void *Imprimir2 () { //Metodo para la impresora 1
	bool Encontre;
	int Ciclo;
	int I;
	int S;
	
	for (Ciclo = 0; Ciclo < 60; Ciclo++) { //Esta impresora trabajara durante 60 ciclos
		sem_wait (&BloquearImpresora);
		Encontre = false;
		
		for (I = N - 1; I > -1 && !Encontre; I--) {
            S = sem_trywait (&ColaPrioridades[I]);
			
            if (S == 0) { //Si el usuario necesita la impresora
				//Le digo al usuario a cual entrar
				sem_post (&Impresora2);
				 //Le aviso al usuario que la impresora termino de imprimir
				sem_post (&EsperandoImpresion[I]);
				//Dejo que la otra impresora entre
				sem_post (&BloquearImpresora);
				Encontre = true;
				//Espero a que el usuario termine de usar la impresora
				sem_wait (&Impresora2);
			}
		}
		
		if (Encontre == false) {
			//En caso de no encontrar ningun pedido, le deja intentar a la otra impresora
			sem_post (&BloquearImpresora);
		}
	}
	exit (0);
}

int Requerir (int P) {
	int S;
	//El usuario se ubica en la cola para que lo busquen por prioridad con prioridad P
	sem_post (&ColaPrioridades[P]);
	//El usuario se queda esperando a que una impresora le deje pasar
	sem_wait (&EsperandoImpresion[P]);
	
	//Al recibir respuesta, el usuario toma una impresora
	S = sem_trywait (&Impresora1);
	if (S == 0) {
		return Imp1;
	}
	else {
		S = sem_trywait (&Impresora2);
		if (S == 0) {
			return Imp2;
		}
		else {
			printf ("Ocurrio Un Error En La Sincronizacion.\n");
			exit (1);
		}
	}
}

void Liberar (int I) {
	//El usuario libera la impresora que tomo
	switch (I) {
		case 1: sem_post (&Impresora1); break;
		case 2: sem_post (&Impresora2); break;
	}
}

void *UsuarioSolicita (void *threadarg) { //Metodo donde inician los usuarios
	
	struct ID_Usuario *U;
	U = (struct ID_Usuario *) threadarg;
	
	int Id = U->ID;
	int P = U->Prioridad;
	
	int Imp;
    int C;
	for (C = 0; C < 20; C++) { //Este usuario solicitara durante 20 ciclos
		Imp = Requerir (P);
		printf ("El Usuario %d Requirio La Impresora %d.\n",Id,Imp);
		sleep (3); //Tiempo que tarda antes de liberarla
		printf ("El Usuario %d Libero La Impresora %d.\n",Id,Imp);
		Liberar (Imp);
		sleep (1); //Esperan un poco antes de volver a requerir
	}
	exit (0);
}

int main () {
	
	int I;
	int rc;
	
	sem_init (&Impresora1,0,0);
	sem_init (&Impresora2,0,0);
	sem_init (&BloquearImpresora,0,1);
	for (I = 0; I < N; I++) {
		sem_init (&ColaPrioridades[I],0,0);
		sem_init (&EsperandoImpresion[I],0,0);
	}
	
	pthread_t TUsuarios [N];
	pthread_t TImpresora1;
	pthread_t TImpresora2;
	
	//Crea Impresora 1
	rc = pthread_create (&TImpresora1, NULL, Imprimir1, NULL);
	if (rc) { //ocurrió un error al crear el Thread, reportar
        printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }
	//Crea Impresora 2
	rc = pthread_create (&TImpresora2, NULL, Imprimir2, NULL);
	if (rc) { //ocurrió un error al crear el Thread, reportar
        printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }
	
	for (I = 0; I < N; I++) { //Crea los N hilos usuario
		datosStruct[I].ID = I;
		datosStruct[I].Prioridad = I;
		rc = pthread_create (&TUsuarios[I], NULL, UsuarioSolicita, (void *) &datosStruct[I]);
		if (rc) { //Ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit (-1);
       	}
	}
	
	for (I = 0; I < N; I++){
		pthread_join (TUsuarios[I], NULL);
	}
	pthread_join (TImpresora1,NULL);
	pthread_join (TImpresora2,NULL);
	
	sem_destroy (&Impresora1);
	sem_destroy (&Impresora2);
	sem_destroy (&BloquearImpresora);
	for (I = 0; I < N; I++) {
		sem_destroy (&ColaPrioridades[I]);
		sem_destroy (&EsperandoImpresion[I]);
	}
	
	printf ("Terminaron Todas Las Impresiones.\n");
	
	return 0;
}
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

//EXPLICAR EL PROBLEMA DE INANICION
//
//
//
//
//

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

/*Algoritmos Impresoras Con Prioridad
	
	//Impresora:
	While True, hacer
		Wait(PedidosPendientes)
		Wait(BloquearImpresora)
		Desde la prioridad mas alta a la mas baja (o hasta encontrar un pedido)
			Trywait(ColasPrioridades[I])
				Signal(ImpresoraXTomar)
				Signal(EsperandoImpresion[I])
				Signal(BloquearImpresora)
				Wait(ImpresoraXDejar)
			Else
				Signal(BloquearImpresora)
	
	//UsuarioSolicita:
	While True, hacer
		Requerir
		Sleep 3 Segundos
		Liberar
		Sleep 1 Segundo
	
	//Requerir:
	Signal(ColasPrioridades[PrioridadPropia])
	Signal(PedidosPendientes)
	Wait(EsperandoImpresion[PrioridadPropia])
	Trywait(Impresora1Tomar)
		Retornar ID Impresora 1
	Sino
		Trywait(Impresora2Tomar)
			Retornar ID Impresora 2
		Sino
			Se produjo un error, retornar 1
	
	//Liberar:
	Liberar impresora pasada por parametro (Signal(ImpresoraXDejar))
*/

sem_t Impresora1Tomar; //Semaforo que se usa para que el usuario pueda tomar la impresora 1. Empieza en 0.
sem_t Impresora2Tomar; //Semaforo que se usa para que el usuario pueda tomar la impresora 2. Empieza en 0.
sem_t Impresora1Dejar; //Semaforo que se usa para decir que el usuario termino de usar la impresora 1. Empieza en 0.
sem_t Impresora2Dejar; //Semaforo que se usa para decir que el usuario termino de usar la impresora 2. Empieza en 0.
sem_t PedidosPendientes; //Semaforo que se usa para que las impresoras busquen un pedido y evitar espera ocupada. Empieza en 0.
sem_t BloquearImpresora; //Semaforo que hace que la busqueda de prioridad se haga de a 1. Empieza en 1.
sem_t ColaPrioridades [N]; //Semaforos que indican cuando un usuario esta pidiendo impresora, ordenado por prioridad. Empiezan en 0.
sem_t EsperandoImpresion [N]; //Semaforos que indican que un usuario esta esperando a ser elegido. Empiezan en 0.

struct ID_Usuario {
	int ID;
	int Prioridad;
};
struct ID_Usuario datosStruct [N]; //Estructura usada para definir la ID y prioridad de un usuario.

struct ID_Impresora {
	int ID;
};
struct ID_Impresora datosStructI [2]; //Estructura usada para definir la ID de una impresora

void *Imprimir (void *threadarg) { //Metodo para las impresoras
	struct ID_Impresora *Imp;
	Imp = (struct ID_Impresora *) threadarg;
	
	int Id = Imp->ID;
	
	bool Encontre;
	int I;
	int S;
	
	while (true) {
		//La impresora espera a que exista al menos un pedido
		sem_wait (&PedidosPendientes);
		//Mutex para que solo una impresora entre a la estructura a la vez
		sem_wait (&BloquearImpresora);
		Encontre = false;
		
		for (I = N - 1; I > -1 && !Encontre; I--) {
            S = sem_trywait (&ColaPrioridades[I]);
			
            if (S == 0) { //Si el usuario necesita la impresora
				//Le digo al usuario a cual entrar
				switch (Id) {
					case Imp1: sem_post (&Impresora1Tomar); break;
					case Imp2: sem_post (&Impresora2Tomar); break;
				}
				//Le aviso al usuario que puede pasar a tomar una impresora
				sem_post (&EsperandoImpresion[I]);
				//Dejo que la otra impresora entre
				sem_post (&BloquearImpresora);
				Encontre = true;
				//Espero a que el usuario termine de usar la impresora
				switch (Id) {
					case Imp1: sem_wait (&Impresora1Dejar); break;
					case Imp2: sem_wait (&Impresora2Dejar); break;
				}
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
	int S = 0;
	//El usuario se ubica en la cola para que lo busquen por prioridad con prioridad P
	sem_post (&ColaPrioridades[P]);
	//El usuario indica que hay un pedido mas pendiente
	sem_post (&PedidosPendientes);
	//El usuario se queda esperando a que una impresora le deje pasar
	sem_wait (&EsperandoImpresion[P]);
	
	//Al recibir respuesta, el usuario toma una impresora
	S = sem_trywait (&Impresora1Tomar);
	if (S == 0) {
		return Imp1;
	}
	else {
		S = sem_trywait (&Impresora2Tomar);
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
		case Imp1: sem_post (&Impresora1Dejar); break;
		case Imp2: sem_post (&Impresora2Dejar); break;
	}
}

void *UsuarioSolicita (void *threadarg) { //Metodo donde inician los usuarios
	
	struct ID_Usuario *U;
	U = (struct ID_Usuario *) threadarg;
	
	int Id = U->ID;
	int P = U->Prioridad;
	
	int Imp;
	while (true) {
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
	
	sem_init (&Impresora1Tomar,0,0);
	sem_init (&Impresora2Tomar,0,0);
	sem_init (&Impresora1Dejar,0,0);
	sem_init (&Impresora2Dejar,0,0);
	sem_init (&PedidosPendientes,0,0);
	sem_init (&BloquearImpresora,0,1);
	for (I = 0; I < N; I++) {
		sem_init (&ColaPrioridades[I],0,0);
		sem_init (&EsperandoImpresion[I],0,0);
	}
	
	pthread_t TUsuarios [N];
	pthread_t TImpresora1;
	pthread_t TImpresora2;
	
	//Crea el hilo Impresora 1
	datosStructI[0].ID = Imp1;
	rc = pthread_create (&TImpresora1, NULL, Imprimir, (void *) &datosStructI[0]);
	if (rc) { //ocurrió un error al crear el Thread, reportar
        printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }
	//Crea el hilo Impresora 2
	datosStructI[1].ID = Imp2;
	rc = pthread_create (&TImpresora2, NULL, Imprimir, (void *) &datosStructI[1]);
	if (rc) { //ocurrió un error al crear el Thread, reportar
        printf ("ERROR; Código de retorno: %d\n", rc);
        exit (-1);
    }
	
	//Crea los N hilos usuario
	for (I = 0; I < N; I++) {
		datosStruct[I].ID = I;
		datosStruct[I].Prioridad = I;
		rc = pthread_create (&TUsuarios[I], NULL, UsuarioSolicita, (void *) &datosStruct[I]);
		if (rc) { //Ocurrió un error al crear el Thread, reportar
        	printf ("ERROR; Código de retorno: %d\n", rc);
        	exit (-1);
       	}
	}
	
	pthread_join (TImpresora1,NULL);
	pthread_join (TImpresora2,NULL);
	for (I = 0; I < N; I++){
		pthread_join (TUsuarios[I], NULL);
	}
	
	sem_destroy (&Impresora1Tomar);
	sem_destroy (&Impresora2Tomar);
	sem_destroy (&Impresora1Dejar);
	sem_destroy (&Impresora2Dejar);
	sem_destroy (&PedidosPendientes);
	sem_destroy (&BloquearImpresora);
	for (I = 0; I < N; I++) {
		sem_destroy (&ColaPrioridades[I]);
		sem_destroy (&EsperandoImpresion[I]);
	}
	
	printf ("Terminaron Todas Las Impresiones.\n");
	
	return 0;
}
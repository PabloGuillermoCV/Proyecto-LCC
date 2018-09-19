#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

/*Algoritmo

	NOTA: Es necesario implementar una ED Cola (tal vez? creo que se puede hacer con Semáforos de Conteo)

	para el Estudiante:

		si AD está dormido
			Despertar (esto se hace con un Signal)
			Requerir Acceso a la Oficina
				//Ser Atendido//
			Liberar Oficina
			Salir
		SINO
			Requerir Acceso y entrar a la cola de Espera
				Si soy el primero en la Cola
					Dejar la Cola de Espera
					Requerir Acceso a la Oficina
						//Ser Atendido//
				Salir

	Para el AD

		Si NO hay estudiantes en la Cola
			Dormir
		Sino
			Sacar un alumno de la Cola y Atenderlo
*/


int main(){
	
}
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

/*Algoritmo

	//Entry
		id = requerir(impresora)
		mientras no tengo el lock, esperar
	//Sección Crítica, obtuve el lock
		//hacer trabajo
	//Exit
		liberar(id)

*/

int main(){


}
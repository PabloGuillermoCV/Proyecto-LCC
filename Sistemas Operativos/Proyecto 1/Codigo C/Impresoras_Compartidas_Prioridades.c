#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>

/*Algoritmo
	
	//Entry
		id = Requerir(Impresora)
		mientras no tengo el lock{
			prioridad++ //Evito Starvation
			dormir 1 segund
		}
	//Sección Critica, obtuve el lock
		Trabajar
	//Exit
		liberar(id);

*/

semaphore S = 0;


int main(){
	
}
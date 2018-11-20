#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<semaphore.h>

#define N 6

/*
Impresoras con prioridad:

+) Se implementan impresoras como hilos
+) Se implementa la prioridad

-) Mal sincronización
-) No se implementa la función liberar, ni se modela
-) Compila con warnings
-) Cast a tipo inexistente
-) No ejecuta. Termina con error sin imprimir nada.
*/

void Requerir () {
	
}

void Liberar () {
	
}

int main () {
	
	
	
	return 0;
}
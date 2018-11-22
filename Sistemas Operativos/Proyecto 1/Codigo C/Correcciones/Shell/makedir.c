#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
//Includes necesarios para simular mkdir
#include <conio.h> 
#include <dir.h> 
#include <process.h> 
#include "Params.h"

int main(){

	args *Par;
	int check;
	char *dirname = Par->nombre; //obtengo el nombre del directorio a crear

	check = mkdir(dirname); //ES UNA SYSCALL, preguntar
	if(!check){
		printf("El directorio %s fue creado en la ruta actual con éxito\n",dirname);
	}
	else{
		fprintf(stderr,"Codigo de error %d, el cual significa %s. Ocurrió un error al crear el directorio %s \n",
					errno,strerror(errno),path);
	}

	return 0;
}
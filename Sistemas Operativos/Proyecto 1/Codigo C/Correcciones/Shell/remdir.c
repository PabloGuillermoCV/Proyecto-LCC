#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
//Includes necesarios para simular rmdir
#include <conio.h> 
#include <dir.h> 
#include <process.h> 

int main(int argc, char *argv[]){

	int check;
	DIR *d;
	char *path = argv[1];
	d = opendir(path);
	if(d == NULL){
		printf("Error -3: El directorio dado NO se encuentra en las inmediaciones\n");
		return 0;
	}
	else{
		closedir(d); //ANTES de continuar, cierro el directorio
		//ESTOY ASUMINEDO QUE EL DIRECTORIO YA ESTA VACIO PARA LLAMAR A LA SYSCALL!
		check = rmdir(path);
		if(check < 0){
			fprintf(stderr,"Codigo de error %d, el cual significa %s, ocurrió un error al borrar el directorio %s \n",
					errno,strerror(errno),path);
			return 0;
		}
		else{
			printf("Se ha eliminado el directorio %s con exito\n",path);
			return 0;
		}
	}
	return 0;

}
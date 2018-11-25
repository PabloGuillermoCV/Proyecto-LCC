#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_BUF 1000


char *Comandos_Disponibles[] = {
	"makedir", //Crear directorio
	"remdir", //Eliminar Directorio
	"cat", //Crear archivo
	"ls", //Listar contenidos de un directorio
	"more", //Mostrar contenido de un archivo
	"exit", //salir
	"help" //comando ayuda
};

/*
 * Método que verifica si el comando obtenido es alguno de los comandos disponibles
*/
int esLegal(char*str){
	bool es =  false;
	int i = 0;
	for(i = 0; i < 7; i++){
		if(strcmp(Comandos_Disponibles[i], str) == 0) //Uso string Compare
			es = true;
	}
	if(!es)
        i = -1;
    else
        i = 0;

	return i;
}

void help(){
	printf("Shell en C para el Primer Proyecto de Sistemas Operativos\n");
	printf("Ingrese alguno de los comandos disponibles con sus parametros correspondientes si los necesitase y presione ENTER\n");
	printf("A continuación se detallan los comandos disponibles:\n");
	printf("makedir [NombreDir] -> Crea un directorio con Nombre NombreDir en la posición actual\n");
	printf("remdir [NombreDir] -> Intenta Eliminar un directorio con NombreDir, solo podrá eliminarse si el directorio esta vacio\n");
	printf("cat [NombreAR] -> Intenta crear un Archivo con nombre NombreAR\n");
	printf("ls -> Lista los contenidos del directorio actual\n");
	printf("more [NombreAR] -> Muestra el contenido del archivo NombreAR\n");
	printf("exit -> termina la ejecución de esta consola\n");
	printf("help -> muestra el presente mensaje\n");
}


int main(){

	char * buffer[MAX_BUF]; //para obtener la linea completa
	char *token;
	char *name;
	bool corte = false; //boolean de corte para salir del programa
	bool error = false; //boolean para errores
	int errno;
	while(!corte){
		printf("pr1> ");
		scanf("%s", &buffer); //leo la linea, "pr1>" NO APARECE en strtok
		//Obtengo un substring, para ver que comando es
		token = strtok(buffer," ");
		if(esLegal(token) == 0){ //Pregunto que el comando dado sea legal
			if(strcmp(token,"help") == 0) //si es "help" o "exit" actuo acordemente
				help();
			else
				if(strcmp(token,"exit") == 0){
					corte = true;
					printf("Se ha salido de la consola con exito, que tenga un buen dia!\n");
				}
				else{
					name = strtok(buffer," "); //obtengo el segundo parámetro, que es un nombre
					strtok(NULL," "); //hago strtok de NULL para cortar, probar contra datos de más

					if(!corte){

						//si llegué aquí, el comando Y los flags son validos, puedo ir a crear el proceso hijo y ejecutar
						int id = fork();
						if(id == 0){ //estoy en el hijo, debo hacer execl ya que paso un unico string, execv necesita un arreglo de strings
                            char *llamada;
                            char *h = "./";
							llamada = strcat(h, token); //Tengo que ejecutar mi propio archivo
							errno = execl(llamada,name); //PROBAR
							printf("El comando se ejecuto con exito\n");
							if(errno < 0){
								fprintf(stderr,"Error al intentar llamar a la funcion %s con codigo de error %d, cuyo significado es: %s \n",
									token,errno,strerror(errno));
							}
						}
						else{ //estoy en el padre, debo esperar
							wait(NULL);
						}
					}

				}
		}
		else{
			printf("Error al procesar el comando, NO es un comando valido, por favor, consulte la ayuda usando el comando help\n ");
		}

	}


}

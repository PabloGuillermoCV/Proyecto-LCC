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
	"mkdir", //Crear directorio
	"rmdir", //Eliminar Directorio
	"cat", //Crear archivo / Mostrar el contenido de un archivo
	"ls", //Listar contenidos de un directorio
	"more", //Mostrar contenido de un archivo
	"exit", //salir
	"help" //comando ayuda
};

char flags[] = {'-','h', 'v', 'a', 'l', 'o', 'q', 'r', 'f'};

int esLegal(char*str){
	bool es =  false;
	int i = 0;
	for(i = 0; i < 7; i++){
		if(strcmp(Comandos_Disponibles[i], str))
			es = true;
	}

	return i;
}

bool esFlagLegal(char c){
	bool ret = false;
	int i;
	for(i = 0; i < 9 && !ret; i++){
		ret = c == flags[i];
	}

	return ret;
}

void help(){
	printf("Shell en C para el Primer Proyecto de Sistemas Operativos\n");
	printf("Ingrese alguno de los comandos disponibles y presione ENTER\n");
	printf("A continuación se detallan los comandos disponibles\n");
	printf("mkdir [NombreDir] Crea un directorio con Nombre NombreDir en la posición actual\n");
	printf("rmdir [NombreDir] Intenta Eliminar un directorio con NombreDir, solo podrá eliminarse si el directorio esta vacio\n");
	printf("cat [NombreAR] Intenta crear un Archivo con nombre NombreAR\n");
	printf("ls [Args] Lista los contenidos del directorio actual, dependiendo de los Args dados\n");
	printf("more [NombreAR] Muestra el contenido del archivo NombreAR\n");
	printf("exit termina la ejecución de esta consola\n");
	printf("help muestra el presente mensaje\n");
}


int main(){

	char * buffer[MAX_BUF]; //para obtener la linea completa
	char * substr[6]; //substring para ver le comando
	char * llamada[11]; //string final del archivo a ejecutar por execv
	bool corte = false; //boolean de corte para salir del programa
	bool error = false; //boolean para errores
	int errno = 0;
	int i = 3;
	printf("pr1> ");
	while(!corte){
		scanf("%s", &buffer); //leo la linea, estoy asumiendo que el "pr1>" NO APARECE
		//Obtengo un substring, para ver que comando es
		strncpy(substring,buffer,5);
		substring[5] = '\n'; //Le tengo que agregar el NULL terminator
		if(esLegal(substring)){

			if(strcmp(substring,"help"))
				help();
			else
				if(strcmp(substring,"exit")){
					corte = true;
					printf("Se ha salido de la consola con exito, que tenga un buen dia!");
				}
				else{
					i = strlen(substring) + 1; //salto al fin del nombre del comando y pasado el espacio de separacion, deberia de encontrar flags o \n
					while(buffer[i] != '\n' && !error && !corte){
						if(buffer[i] != ' ' & !esFlagLegal(buffer[i]))
							error = true;
					}
					//saliendo del while, puede ser o porque todo es legal o porque hubo un error, debo verlo
					if(error){
						printf("Alguno de los flags ingresados NO es correcto, los flags permitidos son: \n");
						printf("-h, -v, -a, -l, -o, -la, -q, -r, -f\n");
					}
					else{
						if(!corte){
							//si llegué aquí, el comando Y los flags son validos, puedo ir a crear el proceso hijo y ejecutar
							int id = fork();
							if(PID == 0){ //estoy en el hijo, debo hacer execv
								llamada = strcat("/bin/",substring);
								errno = execv(llamada,buffer);
								if(errno < 0){
									fprintf(stderr,"Error al intentar llamar a la funcion %s con codigo de error %d, cuyo significado es: %s \n",substring,errno,strerror(errno));
								}
							}
							else{ //estoy en el padre, debo esperar
								wait(NULL);
							}
						}
					}
				}
		}
		else{
			printf("Error al procesar el comando, NO es un comando valido, por favor, consulte la ayuda usando el comando "help"\n ");
		}

	}


}
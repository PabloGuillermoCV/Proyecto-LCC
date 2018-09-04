#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

#define MAX_LINE 80 //Largo máximo para un comando

int main(void){

	char *argumentos[MAX_LINE/2 + 1]; //Argumentos de la Linea de Comandos
	int debe_correr = 1; //Flag para determinar si debo salir o no, 1 = "Debo Correr"

	while(debe_correr){
		printf("pr1>"); //imprimo una linea para nuestro shell
		fflush(stdout); //hago flush del stdout

		/*	Como seguir?
		*	 	
		*	1) Definir los comandos pedidos por el proyecto
				1.1)cada comando deberia tener un pseudónimo asociado
				1.2)cada comando deberia rederigir a una función/procedimiento
				1.3)definir Argumentos legales para la shell y sus modificadores (archivos, '&' , '|', '-x' , etc)
			2) Hacer que la shell lea el input dado
				2.1) Debe darse cuenta que comando fue el llamado y NUESTRO Shell debe PROCESAR los modificadores asociados
				2.2) Hecho esto, se delega en el System Call Correspondiente (mkdir, deldir, etc)
					2.2.1) el shell debe crear un hijo con Fork(), y luego el hijo debe ejecutar execvp(char *command, char *params[])
			3) Consideraciones
				3.1) cuando el usuario ingrese "exit", debe_correr se setea a 0 y el programa debe terminar
				3.2) char *argumentos guardará el contenido del comando ingresado por pantalla, un lugar por "token" 
					(1 token es el nombre del comando, otro token es un modificador como "-la", etc)
		*/
	}
}
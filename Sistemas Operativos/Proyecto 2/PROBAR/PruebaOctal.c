#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcnt1.h>
#include <bool.h>
#include <string.h>
#include <errno.h>


char ALLOWED_OCTAL[6] = {7,5,4,2,1,0} //Arreglo Global para saber los valores permitidos en octal para chmod
char ALLOWED_PERMS[3] = {'r','w','x'} //Arreglo Global para saber los caracteres legales PARA LOS PERMISOS
char ALLOWED_GROUPS[4] = {'u','g','o','a'} //Arreglo Global para saber los caracteres legales PARA LOS USUARIOS
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera
//DEFINO TIPOS DE ERRORES QUE PUEDEN PRODUCIRSE AL PARSEAR LOS DATOS
#define ERR_EXTRA_PARAMS -1
#define ERR_INSUFFICIENT_PARAMS -2
#define ERR_WRONG_PERM -3

/*
 * Función para explicitar los mensajes de error propios asociados a la ejecución de este programa
*/
char* MensError(int n){

	char *ret;

	switch(n){
		case -1:
			ret = "Los parametros presentados sobran\n";
			break;
		case -2:
			ret = "NO hay parametros suficientes para la llamada a chmod\n";
			break;
		case -3:
			ret = "Codigo de permisos en sistema de representacion Octal ilegal\n";
			break;
	}

	return ret;
}
/*
 * Método para determinar que un número pertenezca a los valores permitidos en Octal de chmod
*/
int PerteneceNums(char n){
	int i;
	int ret = 0;
	for(i = 0; i < NELEMS(ALLOWED_OCTAL) && ret == 0; i++){
		if(n == ALLOWED_OCTAL[i]){
			ret = 1;  //El numero obtenido es un octal legal para chmod
		}
	}
	if(ret != 1)
		ret = ERR_WRONG_PERM; //Codigo de error propio, "Código de permisos Ilegal"

	return ret;
}

int main(){

	char p1 = '7';
	char p2 = '9';
	printf("caso de prueba 1 con caracter: %c , el resultado es: %d\n", p1, PerteneceNums(p1));
	printf("Caso de prueba 2 con caracter: %c , el resultado es: %d \n", p2, PerteneceNums(p2));

}
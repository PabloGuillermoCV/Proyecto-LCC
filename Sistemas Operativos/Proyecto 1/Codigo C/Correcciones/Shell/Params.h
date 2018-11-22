//Estructura compartida para el manejo de la Shell. PREGUNTAR SI ES NECESARIO
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>

//Struct especial compartido en la Shell para el pasaje de parámetros
struct Parameters{
	char *nombre;
}
typedef struct Parameters args;
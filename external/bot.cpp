#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "DatosMemCompartida.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char* argv[])
{
	int i;
	DatosMemCompartida* dat;
	char *direccion;
	int fd;
	
	fd=open(X1);
	if (fd == -1)
	{
		perror("Error al abrir el archivo"); 
		return 1;
	}
	direccion=mmap(X2);
	if (direccion == MAP_FAILED)
	{
		perror("Error en la proyeccion mmap"); 
		return 1;
	}
	close(fd);
	dat=(DatosMemCompartida*)direccion;

	while(1)
	{
		usleep(25000);
		// X3 Completar la lógica
		// dat->jugador, por ejemplo, accede al identificador del jugador
	}
	return 0;
}
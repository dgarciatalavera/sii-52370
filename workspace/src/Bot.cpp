#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "Bot.h"

CBot::CBot()
{
    datos_mem = NULL;
}

CBot::~CBot()
{
    //Liberar la proyección de memoria al destruir el objeto bot
    if (datos_mem != NULL && datos_mem != MAP_FAILED) {
        munmap(datos_mem, sizeof(DatosMemCompartida));
    }
}

bool CBot::Init()
{
    //1. Abrir el fichero de tenis
    int fd = open("/tmp/mem_tenis", O_RDWR);
    if (fd == -1) {
        perror("ERROR (Bot): No se pudo abrir /tmp/mem_tenis");
        return false;
    }

    //Proyectar el fichero en memoria compartida mmap
    datos_mem = (DatosMemCompartida *) mmap(
        NULL, 
        sizeof(DatosMemCompartida), 
        PROT_READ | PROT_WRITE,   //Permite leer y escrbir
        MAP_SHARED,    //Los cambios son visibles para otros procesos
        fd, 
        0
    );

    if (datos_mem == MAP_FAILED) {
        perror("ERROR (Bot): Fallo en mmap");
        close(fd);
        return false;
    }

    //Cerrar el descriptor del fichero
    close(fd);
    
    
    printf("BOT: Conectado con exito a la memoria compartida.\n");
    return true;
}

void CBot::ControlaRaqueta()
{
    if (datos_mem == NULL) return;

    //Obtener el punto medio Y de la raqueta
    float centro_raqueta_y = (datos_mem->raqueta1.y1 + datos_mem->raqueta1.y2) / 2.0f;
    float pelota_y = datos_mem->esfera.centro.y;

    
    if (pelota_y > centro_raqueta_y + 0.1f) {
        datos_mem->accion = 1;  // La pelota esta mas arriba, ordena subir
    } else if (pelota_y < centro_raqueta_y - 0.1f) {
        datos_mem->accion = -1; // La pelota esta mas abajo, ordena bajar
    } else {
        datos_mem->accion = 0;  // La pelota esta alineada, ordena parar
    }
}

void CBot::BuclePrincipal()
{
    //Bucle continuo de control
    while (1) {
        usleep(25000); // 25 ms
        ControlaRaqueta();
    }
}

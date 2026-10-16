
// Tuberia FIFO

//Añado librerías necesarias para la manipulación del sistema FIFO
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {
    int fd;
    char buffer[1024];  //Bloque de memoria 1024 bytes para ir guardando las letras y frases que se vayan leyendo de la tubería antes de imprimirlas

    // borrar FIFO por si existiera previamente, Importante por seguridad
    unlink("/tmp/fifotenis");

    /* crear el FIFO */
    if (mkfifo("/tmp/fifotenis",0666)<0) {   //Creo el fichero especial de tipo tuberia en la ruta tmp/fifotenis
        perror("No puede crearse el FIFO");  //En caso de producirse error imprime en pantalla 
        return(1);
    }
    /* Abrir el FIFO */
    if ((fd = open("/tmp/fifotenis", O_RDONLY)) < 0) {  //Abre la tuberia en modo solo lectura
        perror("No puede abrirse el FIFO");
        return(1);
    }
    /* Leer de la FIFO */
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    
    printf("Fin de la transmisión. Cerrando logger.\n");
    
    close(fd);
    unlink("/tmp/fifotenis");
    return(0);
}

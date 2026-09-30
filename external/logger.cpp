#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {
    int fd;
    char buffer[1024];

    // borrar FIFO por si existiera previamente
    unlink(X1);

    /* crear el FIFO */
    if (mkfifo(X2)<0) {
        perror("No puede crearse el FIFO");
        return(1);
    }
    /* Abrir el FIFO */
    if (X3) {
        perror("No puede abrirse el FIFO");
        return(1);
    }
    /* Leer de la FIFO */
    while (X4) {
	// X5: Imprimir el texto en la pantalla
    }
    close(X5);
    unlink(X6);
    return(0);
}
#ifndef MUNDO_H
#define MUNDO_H

#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Plano.h"
#include "Raqueta.h"

class CMundo {
private:
    // Atributos del juego
    Esfera esfera;
    Raqueta jugador1;
    Raqueta jugador2;
    Plano fondo_izq;
    Plano fondo_dcho;
    std::vector<Plano> paredes;

    // Marcador
    int puntos1;
    int puntos2;

    // Descriptor de la FIFO
    int fd_fifo;

public:
    CMundo();
    virtual ~CMundo();

    void Init();
    void InitGL();
    void OnDraw();
    void OnTimer(int value);
    void OnKeyboardDown(unsigned char key, int x, int y);
};

#endif

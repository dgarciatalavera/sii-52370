#pragma once
#include "DatosMemCompartida.h"

class CBot
{
private:
    DatosMemCompartida *datos_mem; //Puntero a la memoria compartida

public:
    CBot();
    ~CBot();

    bool Init();    //Abre el archivo /tmp/mem_tenis y ejecuta mmap
    void ControlaRaqueta();   //Calcula si la raqueta debe subir, bajar o detenerse
    void BuclePrincipal();   //Bucle infinito con usleep(25000)
};

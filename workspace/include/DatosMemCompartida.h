#pragma once 
#include "Esfera.h"
#include "Raqueta.h"

class DatosMemCompartida
{       
public:         
    Esfera esfera;   //Copia del estado actual de la pelota
    Raqueta raqueta1;  //Copia del estado actual de la raqueta izquierda
    int accion; // Ordem bot: 1 arriba, 0 nada, -1 abajo
};

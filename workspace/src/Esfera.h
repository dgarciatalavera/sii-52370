#include "Vector2D.h"

class Esfera  
{
public:
	Vector2D centro;
	Vector2D velocidad;
	float radio;

	Esfera();
	virtual ~Esfera();

	void Dibuja();
	void Mueve(float t);
};

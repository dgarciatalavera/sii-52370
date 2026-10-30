// Mundo.cpp: implementation of the CMundo class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "Mundo.h"
#include "glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo()
{
        datos_mem = NULL;
	Init();
}

CMundo::~CMundo()
{
    if (fd_fifo != -1) { 
        close(fd_fifo);
    }
    
    if (datos_mem != NULL  && datos_mem != MAP_FAILED){
        munmap(datos_mem,sizeof(DatosMemCompartida)); //Desproyectar regio n
    }
}

void CMundo::InitGL()
{
	//Habilitamos las luces, la renderizacion y el color de los materiales
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, 800/600.0f, 0.1, 150);
}

void print(char *mensaje, int x, int y, float r, float g, float b)
{
	glDisable (GL_LIGHTING);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(r,g,b);
	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
	int len = strlen (mensaje );
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
void CMundo::OnDraw()
{
	//Borrado de la pantalla	
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0.0, 0, 17,  // posicion del ojo
		0.0, 0.0, 0.0,      // hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	char cad[100];
	sprintf(cad,"Jugador1: %d",puntos1);
	print(cad,10,0,1,1,1);
	sprintf(cad,"Jugador2: %d",puntos2);
	print(cad,650,0,1,1,1);
	int i;
	for(i=0;i<paredes.size();i++)
		paredes[i].Dibuja();

	fondo_izq.Dibuja();
	fondo_dcho.Dibuja();
	jugador1.Dibuja();
	jugador2.Dibuja();
	esfera.Dibuja();

	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}

void CMundo::OnTimer(int value)
{	
        // Comunicación con el Bot
        if (datos_mem != NULL) {
        //Actualiza la posición de la esfera y la raqueta
            datos_mem->esfera = esfera;
            datos_mem->raqueta1 = jugador1;

          //Aplica la velocidad a jugador1 (izq) segun la orden del bot
            if (datos_mem->accion == 1) {
                jugador1.velocidad.y = 4.0f;  // Subir
            } else if (datos_mem->accion == -1) {
                jugador1.velocidad.y = -4.0f; // Bajar
            } else {
                jugador1.velocidad.y = 0.0f;  // Parar
            }
        }
        
        
        
        
        char buffer[256];
	jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	esfera.Mueve(0.025f);
	int i;
	for(i=0;i<paredes.size();i++)
	{
		paredes[i].Rebota(esfera);
		paredes[i].Rebota(jugador1);
		paredes[i].Rebota(jugador2);
	}

	jugador1.Rebota(esfera);
	jugador2.Rebota(esfera);
	if(fondo_izq.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=2+2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=2+2*rand()/(float)RAND_MAX;
		puntos2++;
		
		
		printf("TENIS: Intento escribir punto 2. fd_fifo=%d\n", fd_fifo);
    fflush(stdout);
    
		if (fd_fifo != -1) {
                  snprintf(buffer, sizeof(buffer), "Jugador 2 marca 1 punto, lleva un total de %d puntos.\n", puntos2);
                  write(fd_fifo, buffer, strlen(buffer));
                  fsync(fd_fifo);
                }
                
                if (puntos2 >= 3) {
                    if (fd_fifo != -1) {
                        close(fd_fifo); // Cierre explícito de la FIFO antes de salir
                    }
                    exit(0); // Finaliza la ejecución del juego
                }
	}

	if(fondo_dcho.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=-2-2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=-2-2*rand()/(float)RAND_MAX;
		puntos1++;
		
		if (fd_fifo != -1) {
                  snprintf(buffer, sizeof(buffer), "Jugador 1 marca 1 punto, lleva un total de %d puntos.\n", puntos1);
                  write(fd_fifo, buffer, strlen(buffer));
                  fsync(fd_fifo);
                }
                
                // Comprobación de fin de partida
                if (puntos1 >= 3) {
                    if (fd_fifo != -1) {
                        close(fd_fifo); // Cierre explícito de la FIFO antes de salir
                    }
                exit(0); // Finaliza la ejecución del juego
                }
	}

}

void CMundo::OnKeyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
//	case 'a':jugador1.velocidad.x=-1;break;
//	case 'd':jugador1.velocidad.x=1;break;
	case 's':jugador1.velocidad.y=-4;break;
	case 'w':jugador1.velocidad.y=4;break;
	case 'l':jugador2.velocidad.y=-4;break;
	case 'o':jugador2.velocidad.y=4;break;

	}
}

void CMundo::Init()
{
	Plano p;
//pared inferior
	p.x1=-7;p.y1=-5;
	p.x2=7;p.y2=-5;
	paredes.push_back(p);

//superior
	p.x1=-7;p.y1=5;
	p.x2=7;p.y2=5;
	paredes.push_back(p);

	fondo_izq.r=0;
	fondo_izq.x1=-7;fondo_izq.y1=-5;
	fondo_izq.x2=-7;fondo_izq.y2=5;

	fondo_dcho.r=0;
	fondo_dcho.x1=7;fondo_dcho.y1=-5;
	fondo_dcho.x2=7;fondo_dcho.y2=5;

	//a la izq
	jugador1.g=0;
	jugador1.x1=-6;jugador1.y1=-1;
	jugador1.x2=-6;jugador1.y2=1;

	//a la dcha
	jugador2.g=0;
	jugador2.x1=6;jugador2.y1=-1;
	jugador2.x2=6;jugador2.y2=1;
	
	
	//Puntos FIFO
	puntos1 = 0;
	puntos2 = 0;

	fd_fifo = open("/tmp/fifotenis", O_WRONLY);
	if (fd_fifo == -1) {
		perror("ERROR al abrir FIFO en tenis");
	} else {
		printf("TENIS: FIFO abierta correctamente (fd=%d)\n", fd_fifo);
		fflush(stdout);
	}
	
	
	
	//Crea o abre el fichero /tmp/mem_tenis 
	int fd_mem = open("/tmp/mem_tenis",O_CREAT | O_RDWR,0666);
	if (fd_mem != -1) {
	  //Dimensiona el fichero al tamaño exacto de la estructura de datos
	    ftruncate(fd_mem, sizeof(DatosMemCompartida));
	    
	    //Proyecta en memoria
	    datos_mem = (DatosMemCompartida *) mmap(
	        NULL,
	        sizeof(DatosMemCompartida),
	        PROT_READ | PROT_WRITE,
	        MAP_SHARED,
	        fd_mem,
	        0
	    );
	    if (datos_mem == MAP_FAILED) {
		perror("ERROR: Fallo en mmap en CMundo::Init");
		datos_mem = NULL;
	    } else {
		// Inicializar la acción en 0, parado
		datos_mem->accion = 0;
	    }
	    close(fd_mem);
	}else {
	    perror("Error: No se puede crear /tmp/mem_tenis");
	}
}

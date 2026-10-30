#include <stdio.h>
#include "Bot.h"

int main()
{
    CBot bot;


    //Intenta conectar con la memoria compartida creada por ./tenis
    if (!bot.Init()) {
        printf("Asegúrate de haber iniciado antes el juego tenis.\n");
        return 1;
    }

    printf("Bot en ejecución controlando raqueta 1...\n");
    bot.BuclePrincipal();  //Entra en el ciclo continuo de toma de decisiones

    return 0;
}

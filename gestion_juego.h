#ifndef GESTION_JUEGO_H 
#define GESTION_JUEGO_H

#include "ronda.h"

#define MAX_RONDAS 5

// ========================================
//       ESTADO GENERAL DE LA PARTIDA
// ========================================
/* Almacena los datos del jugador, el saldo,
   la ronda actual y el resultado de la ronda
*/

typedef struct {
    char nombre[50];           //Nombre del jugador
    int saldo;                 //Dinero disponible
    int numero_ronda;          //Numero de ronda actual
    int ronda_resuelta;        //Bandera que indica si la ronda fue resuelta
    int partida_terminada;     //Bandera que indica si la partida fue resuelta

    Ronda ronda;               //Datos de la apuesta actual
    ResultadoRonda resultado;  //Resultado obtenido al resolverla

} EstadoJuego;

// Inicializa una nueva partida con los datos del jugador
void iniciar_partida(EstadoJuego *juego, const char *nombre);


// Registra una apuesta al pleno, color, paridad y docena
int apostar_pleno(EstadoJuego *juego, int numero, int monto);
int apostar_color(EstadoJuego *juego, const char *color, int monto);
int apostar_paridad(EstadoJuego *juego, const char *paridad, int monto);
int apostar_docena(EstadoJuego *juego, const char *docena, int monto);

// Resuelve las apuestas de la ronda actual
int resolver_ronda_juego(EstadoJuego *juego);

// Avanza a la siguiente ronda
int avanzar_ronda_juego(EstadoJuego *juego);


#endif

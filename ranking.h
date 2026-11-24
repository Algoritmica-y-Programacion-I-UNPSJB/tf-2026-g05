#ifndef RANKING_H
#define RANKING_H

// Cantidad máxima de jugadores que se pueden almacenar en el archivo de ranking.
#define MAX_JUGADORES 10

// Cantidad de jugadores que se muestran en el ranking principal.
#define TOP_RANKING 5


// ==========================================
//          ESTRUCTURA DEL JUGADOR
// ==========================================

// Almacena los datos de cada jugador que forma parte del ranking.
typedef struct {
    char nombre[50]; // Nombre del jugador.
    int puntaje;     // Puntaje obtenido por el jugador.
} Jugador;


// ==========================================
//          FUNCIONES DEL RANKING
// ==========================================

// Guarda el puntaje de un jugador en el archivo de ranking.
// Si el jugador ya existe, actualiza su puntaje si obtuvo uno mejor.
void guardar_puntaje(Jugador jugador);


// Carga los jugadores almacenados en el archivo de ranking.
// Recibe un arreglo donde guardar los jugadores y el máximo de posiciones disponibles.
// Devuelve la cantidad de jugadores cargados.
int cargar_ranking(Jugador jugadores[], int max);


// Carga los primeros jugadores del ranking, según el límite TOP_RANKING.
// Recibe un arreglo donde guardar los jugadores.
// Devuelve la cantidad de jugadores cargados.
int cargar_top_ranking(Jugador jugadores[]);


// Muestra el ranking de jugadores por pantalla.
void mostrar_ranking(void);

#endif

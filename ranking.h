
#ifndef RANKING_H
#define RANKING_H

#define MAX_JUGADORES 10
#define TOP_RANKING 5

typedef struct {
    char nombre[50];
    int puntaje;
} Jugador;

void guardar_puntaje(Jugador jugador);
int cargar_ranking(Jugador jugadores[], int max);
int cargar_top_ranking(Jugador jugadores[]);
void mostrar_ranking(void);

#endif

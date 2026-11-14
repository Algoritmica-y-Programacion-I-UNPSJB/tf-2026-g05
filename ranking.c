
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "ranking.h"

void guardar_puntaje(Jugador jugador) {

    Jugador jugadores[MAX_JUGADORES];
    int cantidad = cargar_ranking(jugadores, MAX_JUGADORES);
    int encontrado = 0;

    // Buscar si el jugador ya existe
    for (int i = 0; i < cantidad; i++) {

        if (strcasecmp(jugadores[i].nombre, jugador.nombre) == 0) {

            encontrado = 1;

            // Conservar solamente su mejor puntaje
            if (jugador.puntaje > jugadores[i].puntaje) {
                jugadores[i].puntaje = jugador.puntaje;
            }

            break;
        }
    }

    // Si es un jugador nuevo
    if (!encontrado) {

        if (cantidad < MAX_JUGADORES) {
            jugadores[cantidad] = jugador;
            cantidad++;
        }
        else if (jugador.puntaje > jugadores[cantidad - 1].puntaje) {
            jugadores[cantidad - 1] = jugador;
        }
        else {
            return;
        }
    }

    // Ordenar de mayor a menor
    for (int i = 0; i < cantidad - 1; i++) {

        for (int j = 0; j < cantidad - 1 - i; j++) {

            if (jugadores[j].puntaje < jugadores[j + 1].puntaje) {

                Jugador aux = jugadores[j];
                jugadores[j] = jugadores[j + 1];
                jugadores[j + 1] = aux;
            }
        }
    }

    // Guardar el ranking actualizado
    FILE *archivo = fopen("ranking.dat", "wb");

    if (archivo == NULL) {
        printf("Error al abrir el archivo del ranking.\n");
        return;
    }

    fwrite(jugadores, sizeof(Jugador), cantidad, archivo);

    fclose(archivo);
}


int cargar_ranking(Jugador jugadores[], int max) {

    FILE *archivo = fopen("ranking.dat", "rb");

    if (archivo == NULL) {
        return 0;
    }

    int cantidad = 0;

    while (cantidad < max &&
           fread(&jugadores[cantidad], sizeof(Jugador), 1, archivo) == 1) {
        cantidad++;
    }

    fclose(archivo);

    // Ordenar de mayor a menor
    for (int i = 0; i < cantidad - 1; i++) {

        for (int j = 0; j < cantidad - 1 - i; j++) {

            if (jugadores[j].puntaje < jugadores[j + 1].puntaje) {

                Jugador aux = jugadores[j];
                jugadores[j] = jugadores[j + 1];
                jugadores[j + 1] = aux;
            }
        }
    }

    return cantidad;
}


int cargar_top_ranking(Jugador jugadores[]) {

    return cargar_ranking(jugadores, TOP_RANKING);
}


void mostrar_ranking(void) {

    Jugador jugadores[TOP_RANKING];

    int cantidad = cargar_top_ranking(jugadores);

    if (cantidad == 0) {
        printf("El ranking esta vacio.\n");
        return;
    }

    printf("\n===== TOP 10 ROYAL ROULETTE =====\n");

    for (int i = 0; i < cantidad; i++) {

        printf("%d. %s - $%d\n",
               i + 1,
               jugadores[i].nombre,
               jugadores[i].puntaje);
    }
}

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "ranking.h"


// ==========================================
//          GUARDAR PUNTAJE
// ==========================================

// Guarda el puntaje de un jugador en el ranking.
// Si el jugador ya existe, conserva solamente su mejor puntaje.
void guardar_puntaje(Jugador jugador) {

    // Arreglo donde se almacenan temporalmente los jugadores.
    Jugador jugadores[MAX_JUGADORES];

    // Carga los jugadores existentes desde el archivo.
    // Devuelve la cantidad de jugadores encontrados.
    int cantidad = cargar_ranking(jugadores, MAX_JUGADORES);

    // Variable que indica si el jugador ya está registrado.
    int encontrado = 0;


    // ==========================================
    //          BUSCAR JUGADOR EXISTENTE
    // ==========================================

    // Recorre los jugadores cargados para buscar si el nombre ya existe.
    for (int i = 0; i < cantidad; i++) {

        // Compara los nombres sin distinguir mayúsculas de minúsculas.
        if (strcasecmp(jugadores[i].nombre, jugador.nombre) == 0) {

            encontrado = 1;

            // Si el nuevo puntaje es mayor, reemplaza al anterior.
            if (jugador.puntaje > jugadores[i].puntaje) {
                jugadores[i].puntaje = jugador.puntaje;
            }

            // Termina la búsqueda porque ya encontró al jugador.
            break;
        }
    }


    // ==========================================
    //             AGREGAR JUGADOR
    // ==========================================

    // Si el jugador no existe, se intenta agregar al ranking.
    if (!encontrado) {

        // Si todavía hay espacio disponible, agrega el jugador al final.
        if (cantidad < MAX_JUGADORES) {
            jugadores[cantidad] = jugador;
            cantidad++;
        }

        // Si el ranking está lleno, reemplaza al jugador con menor puntaje
        // solamente si el nuevo jugador obtuvo un puntaje superior.
        else if (jugador.puntaje > jugadores[cantidad - 1].puntaje) {
            jugadores[cantidad - 1] = jugador;
        }

        // Si no supera el último puntaje, no se guarda.
        else {
            return;
        }
    }


    // ==========================================
    //          ORDENAR EL RANKING
    // ==========================================

    // Ordena los jugadores de mayor a menor puntaje utilizando burbuja.
    for (int i = 0; i < cantidad - 1; i++) {

        for (int j = 0; j < cantidad - 1 - i; j++) {

            // Si el puntaje actual es menor que el siguiente,
            // intercambia sus posiciones.
            if (jugadores[j].puntaje < jugadores[j + 1].puntaje) {

                // Variable auxiliar para guardar temporalmente un jugador.
                Jugador aux = jugadores[j];

                jugadores[j] = jugadores[j + 1];
                jugadores[j + 1] = aux;
            }
        }
    }


    // ==========================================
    //          GUARDAR EN ARCHIVO
    // ==========================================

    // Abre el archivo ranking.dat en modo escritura binaria.
    // Si el archivo existe, reemplaza su contenido anterior.
    FILE *archivo = fopen("ranking.dat", "wb");

    // Comprueba si el archivo se abrió correctamente.
    if (archivo == NULL) {
        printf("Error al abrir el archivo del ranking.\n");
        return;
    }

    // Escribe las estructuras Jugador en el archivo binario.
    // Guarda tantos registros como jugadores haya en el arreglo.
    fwrite(jugadores, sizeof(Jugador), cantidad, archivo);

    // Cierra el archivo para finalizar la escritura.
    fclose(archivo);
}



// ==========================================
//          CARGAR RANKING
// ==========================================

// Lee los jugadores guardados en el archivo binario
// y los almacena en el arreglo recibido como parámetro.
int cargar_ranking(Jugador jugadores[], int max) {

    // Abre el archivo en modo lectura binaria.
    FILE *archivo = fopen("ranking.dat", "rb");

    // Si no puede abrirse, devuelve cero porque no se cargaron jugadores.
    if (archivo == NULL) {
        return 0;
    }

    // Contador de jugadores que se van leyendo.
    int cantidad = 0;

    // Lee las estructuras Jugador mientras haya espacio en el arreglo
    // y fread pueda leer correctamente un registro.
    while (cantidad < max &&
           fread(&jugadores[cantidad], sizeof(Jugador), 1, archivo) == 1) {
        cantidad++;
    }

    // Cierra el archivo después de terminar la lectura.
    fclose(archivo);


    // ==========================================
    //          ORDENAR EL RANKING
    // ==========================================

    // Ordena los jugadores de mayor a menor puntaje utilizando burbuja.
    for (int i = 0; i < cantidad - 1; i++) {

        for (int j = 0; j < cantidad - 1 - i; j++) {

            // Si el puntaje actual es menor que el siguiente,
            // intercambia los jugadores.
            if (jugadores[j].puntaje < jugadores[j + 1].puntaje) {

                // Utiliza una variable auxiliar para realizar el intercambio.
                Jugador aux = jugadores[j];

                jugadores[j] = jugadores[j + 1];
                jugadores[j + 1] = aux;
            }
        }
    }

    // Devuelve la cantidad de jugadores que se cargaron.
    return cantidad;
}



// ==========================================
//          CARGAR TOP DEL RANKING
// ==========================================

// Carga solamente los primeros jugadores del ranking,
// utilizando el límite definido por TOP_RANKING.
int cargar_top_ranking(Jugador jugadores[]) {

    // Llama a cargar_ranking pasando como máximo TOP_RANKING jugadores.
    return cargar_ranking(jugadores, TOP_RANKING);
}



// ==========================================
//          MOSTRAR RANKING
// ==========================================

// Muestra por pantalla los jugadores que forman parte del TOP 5.
void mostrar_ranking(void) {

    // Arreglo que puede almacenar los cinco primeros jugadores.
    Jugador jugadores[TOP_RANKING];

    // Carga el TOP del ranking y obtiene la cantidad de jugadores encontrados.
    int cantidad = cargar_top_ranking(jugadores);

    // Si no hay jugadores registrados, muestra un mensaje y termina.
    if (cantidad == 0) {
        printf("El ranking esta vacio.\n");
        return;
    }

    // Muestra el título del ranking.
    printf("\n===== TOP 5 ROYAL ROULETTE =====\n");

    // Recorre los jugadores cargados y muestra su posición,
    // nombre y puntaje obtenido.
    for (int i = 0; i < cantidad; i++) {

        printf("%d. %s - $%d\n",
               i + 1,
               jugadores[i].nombre,
               jugadores[i].puntaje);
    }
}

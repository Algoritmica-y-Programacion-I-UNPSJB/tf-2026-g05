#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ronda.h"
#include "ruleta.h"
#include "apuesta.h"


// Inicializa una nueva ronda poniendo en cero la cantidad de apuestas
// de cada tipo y el monto total apostado.
void iniciar_ronda(Ronda *ronda) {
    ronda->cantidad_plenos = 0;
    ronda->cantidad_colores = 0;
    ronda->cantidad_paridades = 0;
    ronda->cantidad_docenas = 0;
    ronda->total_apostado = 0;
}


// Registra una apuesta a un número específico (pleno).
// Verifica que el número esté entre 0 y 36, que el monto sea válido,
// que el jugador tenga saldo disponible y que no se supere el límite de apuestas.
// Devuelve 1 si la apuesta se registra correctamente y 0 si es inválida.
int registrar_apuesta_pleno(Ronda *ronda, int numero, int monto, int saldo) {

    if (numero < 0 || numero > 36) {
        return 0;
    }

    if (monto <= 0 || monto > saldo - ronda->total_apostado) {
        return 0;
    }

    if (ronda->cantidad_plenos >= MAX_APUESTAS) {
        return 0;
    }

    // Obtiene la posición libre donde se guardará la nueva apuesta.
    int posicion = ronda->cantidad_plenos;

    // Guarda el número elegido y el monto apostado.
    ronda->numeros_apostados[posicion] = numero;
    ronda->montos_pleno[posicion] = monto;

    // Actualiza la cantidad de apuestas y el total apostado.
    ronda->cantidad_plenos++;
    ronda->total_apostado += monto;

    return 1;
}


// Registra una apuesta a un color (rojo o negro).
// Comprueba que el color sea válido, que el monto no supere el saldo disponible
// y que exista espacio para guardar la apuesta.
// Devuelve 1 si se registra correctamente y 0 si no cumple las condiciones.
int registrar_apuesta_color(Ronda *ronda, const char *color, int monto, int saldo) {

    if (strcmp(color, "rojo") != 0 && strcmp(color, "negro") != 0) {
        return 0;
    }

    if (monto <= 0 || monto > saldo - ronda->total_apostado) {
        return 0;
    }

    if (ronda->cantidad_colores >= MAX_APUESTAS) {
        return 0;
    }

    // Obtiene la posición libre del arreglo de apuestas de colores.
    int posicion = ronda->cantidad_colores;

    // Guarda el color elegido y el monto apostado.
    strcpy(ronda->colores_apostados[posicion], color);
    ronda->montos_color[posicion] = monto;

    // Actualiza la cantidad de apuestas y el total apostado.
    ronda->cantidad_colores++;
    ronda->total_apostado += monto;

    return 1;
}


// Registra una apuesta a par o impar.
// Valida la opción elegida, el monto disponible y la capacidad del arreglo.
// Devuelve 1 si la apuesta se registra y 0 si es inválida.
int registrar_apuesta_paridad(Ronda *ronda, const char *paridad, int monto, int saldo) {

    if (strcmp(paridad, "par") != 0 && strcmp(paridad, "impar") != 0) {
        return 0;
    }

    if (monto <= 0 || monto > saldo - ronda->total_apostado) {
        return 0;
    }

    if (ronda->cantidad_paridades >= MAX_APUESTAS) {
        return 0;
    }

    // Obtiene la posición libre del arreglo de apuestas de paridad.
    int posicion = ronda->cantidad_paridades;

    // Guarda la paridad elegida y el monto apostado.
    strcpy(ronda->paridades_apostadas[posicion], paridad);
    ronda->montos_paridad[posicion] = monto;

    // Actualiza la cantidad de apuestas y el total apostado.
    ronda->cantidad_paridades++;
    ronda->total_apostado += monto;

    return 1;
}


// Registra una apuesta a una de las tres docenas.
// Comprueba que la docena sea válida, que el monto esté dentro del saldo
// disponible y que no se supere la capacidad del arreglo.
// Devuelve 1 si se registra correctamente y 0 si es inválida.
int registrar_apuesta_docena(Ronda *ronda, const char *docena, int monto, int saldo) {

    if (strcmp(docena, "primera") != 0 &&
        strcmp(docena, "segunda") != 0 &&
        strcmp(docena, "tercera") != 0) {
        return 0;
    }

    if (monto <= 0 || monto > saldo - ronda->total_apostado) {
        return 0;
    }

    if (ronda->cantidad_docenas >= MAX_APUESTAS) {
        return 0;
    }

    // Obtiene la posición libre del arreglo de apuestas de docenas.
    int posicion = ronda->cantidad_docenas;

    // Guarda la docena elegida y el monto apostado.
    strcpy(ronda->docenas_apostadas[posicion], docena);
    ronda->montos_docena[posicion] = monto;

    // Actualiza la cantidad de apuestas y el total apostado.
    ronda->cantidad_docenas++;
    ronda->total_apostado += monto;

    return 1;
}


// Resuelve la ronda: gira la ruleta, comprueba las apuestas,
// calcula los premios y actualiza el saldo del jugador.
// Devuelve 1 si la ronda se resuelve correctamente y 0 si hay un error.
int resolver_ronda(Ronda *ronda, int *saldo, ResultadoRonda *resultado) {

    // Comprueba que los punteros recibidos sean válidos.
    if (ronda == NULL || saldo == NULL || resultado == NULL) {
        return 0;
    }

    // Comprueba que existan apuestas y que el total no supere el saldo.
    if (ronda->total_apostado <= 0 || ronda->total_apostado > *saldo) {
        return 0;
    }

    // ==========================================
    //             GIRAR LA RULETA
    // ==========================================

    // Obtiene el número que salió en la ruleta.
    int numero_salido = girar_ruleta();

    // Determina el color, la paridad y la docena del número obtenido.
    char *color_salido = color_numero(numero_salido);
    char *paridad_salida = paridad(numero_salido);
    char *docena_salida = docena_numero(numero_salido);

    // Guarda los datos obtenidos en la estructura del resultado.
    resultado->numero_salido = numero_salido;

    strcpy(resultado->color_salido, color_salido);
    strcpy(resultado->paridad_salida, paridad_salida);
    strcpy(resultado->docena_salida, docena_salida);

    // Guarda cuánto dinero se apostó en total durante la ronda.
    resultado->total_apostado = ronda->total_apostado;

    // Acumulará el dinero que el jugador recibe por las apuestas ganadoras.
    int premio_total = 0;


    // ==========================================
    //             RESOLVER PLENOS
    // ==========================================

    // Recorre todas las apuestas a números individuales.
    for (int i = 0; i < ronda->cantidad_plenos; i++) {

        // Comprueba si el número apostado coincide con el número salido.
        if (verificar_pleno(numero_salido, ronda->numeros_apostados[i])) {

            // En el pleno, la ganancia es 35 veces el monto apostado.
            // También se devuelve el monto original de la apuesta.
            int ganancia = ronda->montos_pleno[i] * 35;
            premio_total += ronda->montos_pleno[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER COLORES
    // ==========================================

    // Recorre todas las apuestas a colores.
    for (int i = 0; i < ronda->cantidad_colores; i++) {

        // Comprueba si el color apostado coincide con el resultado.
        if (verificar_color(color_salido, ronda->colores_apostados[i])) {

            // En las apuestas a color, la ganancia es igual al monto apostado.
            // También se devuelve el monto original.
            int ganancia = ronda->montos_color[i];
            premio_total += ronda->montos_color[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER PARIDADES
    // ==========================================

    // Recorre todas las apuestas a par o impar.
    for (int i = 0; i < ronda->cantidad_paridades; i++) {

        // Comprueba si la paridad apostada coincide con el resultado.
        if (verificar_paridad(paridad_salida, ronda->paridades_apostadas[i])) {

            // En las apuestas a paridad, la ganancia es igual al monto apostado.
            // También se devuelve el monto original.
            int ganancia = ronda->montos_paridad[i];
            premio_total += ronda->montos_paridad[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER DOCENAS
    // ==========================================

    // Recorre todas las apuestas a docenas.
    for (int i = 0; i < ronda->cantidad_docenas; i++) {

        // Comprueba si la docena apostada coincide con el resultado.
        if (verificar_docena(docena_salida, ronda->docenas_apostadas[i])) {

            // En las apuestas a docena, la ganancia es el doble del monto apostado.
            // También se devuelve el monto original.
            int ganancia = ronda->montos_docena[i] * 2;
            premio_total += ronda->montos_docena[i] + ganancia;
        }
    }


    // ==========================================
    //              ACTUALIZAR SALDO
    // ==========================================

    // Resta del saldo el total apostado y suma los premios obtenidos.
    *saldo = *saldo - ronda->total_apostado + premio_total;

    // Guarda el premio obtenido y el saldo final en la estructura del resultado.
    resultado->premio_total = premio_total;
    resultado->saldo_final = *saldo;

    return 1;
}

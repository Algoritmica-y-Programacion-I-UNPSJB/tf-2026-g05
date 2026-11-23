#include <stdio.h>
#include "gestion_juego.h"


// ========================================================
//              GESTION GENERAL DE LA PARTIDA
// ========================================================
/* Este archivo administra el estado general del juego.
   inicializa la partida, recibe las apuestas, resuelve las
   rondas y controla si se puede avanzar o si debe finalizar.
*/

// ==========================================
// INICIALIZA LOS DATOS DE UNA NUEVA PARTIDA
// ==========================================
void iniciar_partida(EstadoJuego *juego, const char *nombre) {

    // Verifica que los punteros recibidos sean validos
    if (juego == NULL || nombre == NULL) {
        return;
    }

    // Copia el nombre del jugador sin superar el limite, agregando el terminador al final
    int i = 0;
    while (i < 49 && nombre[i] != '\0') {
        juego->nombre[i] = nombre[i];
        i++;
    }
    juego->nombre[i] = '\0';


    // Inicializa los datos generales de la partida
    juego->saldo = 1000;
    juego->numero_ronda = 1;
    juego->ronda_resuelta = 0;    //bandera si se ha terminado la ronda
    juego->partida_terminada = 0; //bandera se se ha terminado la partida


    // Prepara las apuestas de la primera ronda
    iniciar_ronda(&juego->ronda);


    // Inicializa los datos del resultado de la ronda
    juego->resultado.numero_salido = 0;
    juego->resultado.total_apostado = 0;
    juego->resultado.premio_total = 0;
    juego->resultado.saldo_final = 0;


    // Las cadenas quedan vacias porque todavia no se giro la ruleta
    juego->resultado.color_salido[0] = '\0';
    juego->resultado.paridad_salida[0] = '\0';
    juego->resultado.docena_salida[0] = '\0';
}


// ==========================================================
//                 REGISTRO DE LAS APUESTAS
// ==========================================================
/* Estas funciones comprueban que la partida permita apostar
   y transfieren el registro y las validaciones en
   las funciones registrar_apuesta de ronda.c
   Devuelven 1 si la apuesta se registro correctamente o 0
   si no pudo registrarse.
*/

// ------------------------- PLENO ---------------------------
int apostar_pleno(EstadoJuego *juego, int numero, int monto) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }

    // No permite apostar si la ronda ya fue resuelta
    if (juego->ronda_resuelta == 1) {
        return 0;
    }

    // Registra la apuesta utilizando los datos de la partida
    return registrar_apuesta_pleno(
        &juego->ronda, //direccion de la ronda dentro de la partida para modificar las apuestas
        numero,        //numero elegido
        monto,         //plata apostada
        juego->saldo   //saldo actual
    );
}



// ------------------------- COLOR ---------------------------
int apostar_color(EstadoJuego *juego, const char *color, int monto) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }

    // Verifica que la ronda todavia no haya sido resuelta
    if (juego->ronda_resuelta == 1 || color == NULL) {
        return 0;
    }

    return registrar_apuesta_color(&juego->ronda, color, monto, juego->saldo);
}



// ------------------------- PARIDAD ---------------------------
int apostar_paridad(EstadoJuego *juego, const char *paridad, int monto) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }

    // Verifica que la ronda todavia no haya sido resuelta
    if (juego->ronda_resuelta == 1 || paridad == NULL) {
        return 0;
    }

    return registrar_apuesta_paridad(&juego->ronda, paridad, monto, juego->saldo);
}



// ------------------------- DOCENA ---------------------------
int apostar_docena(EstadoJuego *juego, const char *docena, int monto) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }

    // Verifica que la ronda todavia no haya sido resuelta
    if (juego->ronda_resuelta == 1 || docena == NULL) {
        return 0;
    }

    return registrar_apuesta_docena(&juego->ronda, docena, monto, juego->saldo);
}


// ========================================================
//                  RESUELVE LA RONDA
// ========================================================
/* Ejecuta el giro de la ruleta y calcula el resultado de todas
   las apuestas registradas. Si la resolucion tiene exito,
   actualiza las banderas y comprueba si la partida finalizo.
   Devuelve 1 si se resolvio correctamente o 0 si hubo un error
*/

int resolver_ronda_juego(EstadoJuego *juego) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }

    // Verifica que la ronda todavia no haya sido resuelta
    if (juego->ronda_resuelta == 1) {
        return 0;
    }

    // ronda.c realiza el giro, calcula los premios y actualiza el saldo.
    // Si devuelve 0, la ronda no pudo resolverse
    if (resolver_ronda(&juego->ronda, &juego->saldo, &juego->resultado) == 0) {
        return 0;
    }

    // Marca la ronda como resuelta
    juego->ronda_resuelta = 1;

    // Comprueba si la partida debe finalizar
    if (juego->saldo <= 0 || juego->numero_ronda >= MAX_RONDAS) {
        juego->partida_terminada = 1;
    }

    return 1;
}


// ==========================================================
//               AVANZA A LA SIGUIENTE RONDA
// ==========================================================
/* Solo permite avanzar si la partida sigue activa y la ronda
   actual ya fue resuelta. Prepara una ronda nueva y actualiza
   el numero de ronda.
   Devuelve 1 si pudo avanzar o 0 si no se pudo
*/

int avanzar_ronda_juego(EstadoJuego *juego) {

    // Verifica que la partida exista y no haya terminado
    if (juego == NULL || juego->partida_terminada == 1) {
        return 0;
    }
    // Comprueba que la ronda actual haya sido resuelta
    if (juego->ronda_resuelta == 0) {
        return 0;
    }


    // Evita superar la cantidad maxima de rondas
    if (juego->numero_ronda >= MAX_RONDAS) {
        return 0;
    }

    // Pasa a la siguiente ronda
    juego->numero_ronda++;

    // Reinicia las apuestas para la nueva ronda
    iniciar_ronda(&juego->ronda);

    // Permite que la nueva ronda pueda resolverse
    juego->ronda_resuelta = 0;

    return 1;
}

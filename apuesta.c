#include <string.h>
#include "apuesta.h"

// Compara el número que salió en la ruleta con el número apostado.
int verificar_pleno(int numero_salido, int numero_apostado) {
    if (numero_salido == numero_apostado) return 1;
    else return 0;
}

// Compara el color obtenido con el color elegido por el jugador.
// strcmp devuelve 0 cuando ambas cadenas son iguales.
int verificar_color(char* color_salido, char* color_apostado) {
    if (strcmp(color_salido, color_apostado) == 0) return 1;
    else return 0;
}

// Compara la paridad obtenida (par o impar) con la paridad apostada.
int verificar_paridad(char* paridad_salida, char* paridad_apostada) {
    if (strcmp(paridad_salida, paridad_apostada) == 0) return 1;
    else return 0;
}

// Compara la docena obtenida con la docena elegida en la apuesta.
int verificar_docena(char* docena_salida, char* docena_apostada) {
    if (strcmp(docena_salida, docena_apostada) == 0) return 1;
    else return 0;
}

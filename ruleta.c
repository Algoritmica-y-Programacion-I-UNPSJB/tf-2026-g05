#include <stdlib.h>
#include "ruleta.h"

int girar_ruleta(void) {
    int numero = rand() % 37;
    return numero;
}

char* color_numero(int numero) {
    if (numero == 0) return "verde";
    else if (
        numero == 1 || numero == 3 || numero == 5 ||
        numero == 7 || numero == 9 || numero == 12 ||
        numero == 14 || numero == 16 || numero == 18 ||
        numero == 19 || numero == 21 || numero == 23 ||
        numero == 25 || numero == 27 || numero == 30 ||
        numero == 32 || numero == 34 || numero == 36
    ) return "rojo";
    else return "negro";
}

char* paridad(int numero) {
    if (numero == 0) return "ninguna";
    if (numero % 2 == 0) return "par";
    else return "impar";
}

char* docena_numero(int numero) {
    if (numero == 0) return "ninguna";
    if (numero <= 12) return "primera";
    else if (numero <= 24) return "segunda";
    else if (numero <= 36) return "tercera";
    return "ninguna";
}
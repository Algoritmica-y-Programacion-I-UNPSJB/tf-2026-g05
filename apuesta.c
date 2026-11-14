#include <string.h>
#include "apuesta.h"

int verificar_pleno(int numero_salido, int numero_apostado) {
    if (numero_salido == numero_apostado) return 1;
    else return 0;
}

int verificar_color(char* color_salido, char* color_apostado) {
    if (strcmp(color_salido, color_apostado) == 0) return 1;
    else return 0;
}

int verificar_paridad(char* paridad_salida, char* paridad_apostada) {
    if (strcmp(paridad_salida, paridad_apostada) == 0) return 1;
    else return 0;
}

int verificar_docena(char* docena_salida, char* docena_apostada) {
    if (strcmp(docena_salida, docena_apostada) == 0) return 1;
    else return 0;
}

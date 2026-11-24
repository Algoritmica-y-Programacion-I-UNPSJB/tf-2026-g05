#ifndef APUESTA_H
#define APUESTA_H

// Verifica si el número que salió en la ruleta coincide con el número apostado.
int verificar_pleno(int numero_salido, int numero_apostado);

// Verifica si el color obtenido coincide con el color elegido en la apuesta.
int verificar_color(char* color_salido, char* color_apostado);

// Verifica si la paridad obtenida coincide con la paridad elegida (par o impar).
int verificar_paridad(char* paridad_salida, char* paridad_apostada);

// Verifica si la docena obtenida coincide con la docena elegida.
int verificar_docena(char* docena_salida, char* docena_apostada);

#endif

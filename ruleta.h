#ifndef RULETA_H
#define RULETA_H

// Genera y devuelve un número aleatorio entre 0 y 36.
int girar_ruleta(void);


// Recibe el número que salió en la ruleta y devuelve su color correspondiente
// (rojo, negro o verde).
char* color_numero(int numero);


// Recibe el número obtenido y devuelve su paridad (par o impar).
// El número 0 no pertenece a ninguna de las dos categorías.
char* paridad(int numero);


// Recibe el número obtenido y devuelve a qué docena pertenece
// (primera, segunda o tercera).
char* docena_numero(int numero);

#endif

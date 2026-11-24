


#ifndef RONDA_H
#define RONDA_H

// Cantidad máxima de apuestas que se pueden registrar por cada tipo.
#define MAX_APUESTAS 5000


// ==========================================
//             ESTADO DE LA RONDA
// ==========================================

// Estructura que almacena todas las apuestas realizadas durante una ronda.
typedef struct {

    // Arreglos que guardan los números elegidos y los montos apostados al pleno.
    int numeros_apostados[MAX_APUESTAS];
    int montos_pleno[MAX_APUESTAS];
    int cantidad_plenos; // Cantidad de apuestas al pleno registradas.

    // Arreglos que guardan los colores elegidos y sus respectivos montos.
    char colores_apostados[MAX_APUESTAS][10];
    int montos_color[MAX_APUESTAS];
    int cantidad_colores; // Cantidad de apuestas a colores registradas.

    // Arreglos que guardan las paridades elegidas y sus respectivos montos.
    char paridades_apostadas[MAX_APUESTAS][10];
    int montos_paridad[MAX_APUESTAS];
    int cantidad_paridades; // Cantidad de apuestas a paridad registradas.

    // Arreglos que guardan las docenas elegidas y sus respectivos montos.
    char docenas_apostadas[MAX_APUESTAS][20];
    int montos_docena[MAX_APUESTAS];
    int cantidad_docenas; // Cantidad de apuestas a docenas registradas.

    // Almacena la suma de todos los montos apostados en la ronda.
    int total_apostado;

} Ronda;


// ==========================================
//           RESULTADO DE LA RONDA
// ==========================================

// Estructura que almacena la información obtenida al finalizar una ronda.
typedef struct {

    int numero_salido;       // Número que salió en la ruleta.
    char color_salido[10];   // Color correspondiente al número obtenido.
    char paridad_salida[10]; // Indica si el número es par o impar.
    char docena_salida[20];  // Docena a la que pertenece el número obtenido.

    int total_apostado; // Dinero total que se apostó durante la ronda.
    int premio_total;   // Dinero total que recibe el jugador por sus apuestas ganadoras.
    int saldo_final;    // Saldo del jugador después de resolver la ronda.

} ResultadoRonda;


// ==========================================
//           FUNCIONES DE LA RONDA
// ==========================================

// Inicializa una ronda nueva poniendo en cero las cantidades de apuestas
// y el total apostado.
void iniciar_ronda(Ronda *ronda);


// Registra una apuesta a un número específico.
// Recibe la ronda, el número elegido, el monto y el saldo disponible.
int registrar_apuesta_pleno(Ronda *ronda, int numero, int monto, int saldo);


// Registra una apuesta a un color (rojo o negro).
// Recibe la ronda, el color elegido, el monto y el saldo disponible.
int registrar_apuesta_color(Ronda *ronda, const char *color, int monto, int saldo);


// Registra una apuesta a una paridad (par o impar).
// Recibe la ronda, la paridad elegida, el monto y el saldo disponible.
int registrar_apuesta_paridad(Ronda *ronda, const char *paridad, int monto, int saldo);


// Registra una apuesta a una de las tres docenas.
// Recibe la ronda, la docena elegida, el monto y el saldo disponible.
int registrar_apuesta_docena(Ronda *ronda, const char *docena, int monto, int saldo);


// Gira la ruleta, verifica las apuestas realizadas, calcula los premios
// y actualiza el saldo del jugador.
// Recibe la ronda, un puntero al saldo y un puntero donde guardar el resultado.
// Devuelve 1 si la ronda se resuelve correctamente y 0 si ocurre un error.
int resolver_ronda(Ronda *ronda, int *saldo, ResultadoRonda *resultado);


#endif

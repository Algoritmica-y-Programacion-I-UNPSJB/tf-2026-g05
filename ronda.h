
#ifndef RONDA_H
#define RONDA_H

#define MAX_APUESTAS 5000

// ==========================================
//             ESTADO DE LA RONDA
// ==========================================

typedef struct {
    // Apuestas al pleno
    int numeros_apostados[MAX_APUESTAS];
    int montos_pleno[MAX_APUESTAS];
    int cantidad_plenos;

    // Apuestas al color
    char colores_apostados[MAX_APUESTAS][10];
    int montos_color[MAX_APUESTAS];
    int cantidad_colores;

    // Apuestas a la paridad
    char paridades_apostadas[MAX_APUESTAS][10];
    int montos_paridad[MAX_APUESTAS];
    int cantidad_paridades;

    // Apuestas a la docena
    char docenas_apostadas[MAX_APUESTAS][20];
    int montos_docena[MAX_APUESTAS];
    int cantidad_docenas;

    // Dinero apostado en total
    int total_apostado;

} Ronda;


// ==========================================
//           RESULTADO DE LA RONDA
// ==========================================

typedef struct {
    int numero_salido;
    char color_salido[10];
    char paridad_salida[10];
    char docena_salida[20];

    int total_apostado;
    int premio_total;
    int saldo_final;

} ResultadoRonda;


// ==========================================
//           FUNCIONES DE LA RONDA
// ==========================================

// Prepara una ronda nueva
void iniciar_ronda(Ronda *ronda);

// Registra apuestas
int registrar_apuesta_pleno(Ronda *ronda, int numero, int monto, int saldo);
int registrar_apuesta_color(Ronda *ronda, const char *color, int monto, int saldo);
int registrar_apuesta_paridad(Ronda *ronda, const char *paridad, int monto, int saldo);
int registrar_apuesta_docena(Ronda *ronda, const char *docena, int monto, int saldo);

// Gira la ruleta, resuelve las apuestas y actualiza el saldo
int resolver_ronda(Ronda *ronda, int *saldo, ResultadoRonda *resultado);

// Se conserva la versión de consola
void jugar_ronda(int *saldo, int *hubo_apuestas);

#endif
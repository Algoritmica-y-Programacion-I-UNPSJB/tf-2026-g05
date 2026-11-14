#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ronda.h"
#include "ruleta.h"
#include "apuesta.h"



void iniciar_ronda(Ronda *ronda) {ronda->cantidad_plenos = 0; ronda->cantidad_colores = 0; ronda->cantidad_paridades = 0; 
    ronda->cantidad_docenas = 0;
    ronda->total_apostado = 0;
}



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

    int posicion = ronda->cantidad_plenos;

    ronda->numeros_apostados[posicion] = numero;
    ronda->montos_pleno[posicion] = monto;

    ronda->cantidad_plenos++;
    ronda->total_apostado += monto;

    return 1;
}



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

    int posicion = ronda->cantidad_colores;

    strcpy(ronda->colores_apostados[posicion], color);
    ronda->montos_color[posicion] = monto;

    ronda->cantidad_colores++;
    ronda->total_apostado += monto;

    return 1;
}





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

    int posicion = ronda->cantidad_paridades;

    strcpy(ronda->paridades_apostadas[posicion], paridad);
    ronda->montos_paridad[posicion] = monto;

    ronda->cantidad_paridades++;
    ronda->total_apostado += monto;

    return 1;
}





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

    int posicion = ronda->cantidad_docenas;

    strcpy(ronda->docenas_apostadas[posicion], docena);
    ronda->montos_docena[posicion] = monto;

    ronda->cantidad_docenas++;
    ronda->total_apostado += monto;

    return 1;
}



int resolver_ronda(Ronda *ronda, int *saldo, ResultadoRonda *resultado) {

    if (ronda == NULL || saldo == NULL || resultado == NULL) {
        return 0;
    }

    if (ronda->total_apostado <= 0 || ronda->total_apostado > *saldo) {
        return 0;
    }

    // ==========================================
    //             GIRAR LA RULETA
    // ==========================================

    int numero_salido = girar_ruleta();

    char *color_salido = color_numero(numero_salido);
    char *paridad_salida = paridad(numero_salido);
    char *docena_salida = docena_numero(numero_salido);

    // Guardar el resultado de la ruleta

    resultado->numero_salido = numero_salido;

    strcpy(resultado->color_salido, color_salido);
    strcpy(resultado->paridad_salida, paridad_salida);
    strcpy(resultado->docena_salida, docena_salida);

    resultado->total_apostado = ronda->total_apostado;

    int premio_total = 0;


    // ==========================================
    //             RESOLVER PLENOS
    // ==========================================

    for (int i = 0; i < ronda->cantidad_plenos; i++) {

        if (verificar_pleno(numero_salido, ronda->numeros_apostados[i])) {

            int ganancia = ronda->montos_pleno[i] * 35;
            premio_total += ronda->montos_pleno[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER COLORES
    // ==========================================

    for (int i = 0; i < ronda->cantidad_colores; i++) {

        if (verificar_color(color_salido, ronda->colores_apostados[i])) {

            int ganancia = ronda->montos_color[i];
            premio_total += ronda->montos_color[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER PARIDADES
    // ==========================================

    for (int i = 0; i < ronda->cantidad_paridades; i++) {

        if (verificar_paridad(paridad_salida, ronda->paridades_apostadas[i])) {

            int ganancia = ronda->montos_paridad[i];
            premio_total += ronda->montos_paridad[i] + ganancia;
        }
    }


    // ==========================================
    //             RESOLVER DOCENAS
    // ==========================================

    for (int i = 0; i < ronda->cantidad_docenas; i++) {

        if (verificar_docena(docena_salida, ronda->docenas_apostadas[i])) {

            int ganancia = ronda->montos_docena[i] * 2;
            premio_total += ronda->montos_docena[i] + ganancia;
        }
    }


    // ==========================================
    //              ACTUALIZAR SALDO
    // ==========================================

    *saldo = *saldo - ronda->total_apostado + premio_total;

    resultado->premio_total = premio_total;
    resultado->saldo_final = *saldo;

    return 1;
}




void jugar_ronda(int *saldo, int *hubo_apuestas) {
    int opcion = 0;
    *hubo_apuestas = 0;

    // GUARDA LA SUMA DE TODAS LAS APUESTAS
    int total_apostado = 0;
    // GUARDA LA SUMA DE TODOS LOS PREMIOS
    int premio_total = 0;

    // ==========================================================
    //         VARIABLES PARA GUARDAR APUESTAS DE LA RONDA
    // ==========================================================

    // APUESTAS AL PLENO
    int numeros_apostados[MAX_APUESTAS];
    int montos_pleno[MAX_APUESTAS];
    int cantidad_plenos = 0;

    // APUESTAS AL COLOR
    char colores_apostados[MAX_APUESTAS][10];
    int montos_color[MAX_APUESTAS];
    int cantidad_colores = 0;

    // APUESTAS A LA PARIDAD
    char paridades_apostadas[MAX_APUESTAS][10];
    int montos_paridad[MAX_APUESTAS];
    int cantidad_paridades = 0;

    // APUESTAS A LA DOCENA
    char docenas_apostadas[MAX_APUESTAS][20];
    int montos_docena[MAX_APUESTAS];
    int cantidad_docenas = 0;

    // ==========================================================
    //              VARIABLES TEMPORALES
    // ==========================================================

    int numero_apostado = 0;
    int monto_pleno = 0;
    char color_apostado[10];
    int monto_color = 0;
    char paridad_apostada[10];
    int monto_paridad = 0;
    char docena_apostada[20];
    int monto_docena = 0;

    int numero_salido;
    char *color_salido;
    char *paridad_salida;
    char *docena_salida;

    // ==========================================================
    //                     MENU DE APUESTAS
    // ==========================================================

    // REPITE EL MENU HASTA QUE SE ELIJA EL 5
    do {
        printf("\n--- MENU DE APUESTAS ---\n");
        printf("Saldo disponible: $%d\n", *saldo - total_apostado);
        printf("1. Apostar a un numero (pleno)\n");
        printf("2. Apostar al color\n");
        printf("3. Apostar a la paridad\n");
        printf("4. Apostar a una docena\n");
        printf("5. Terminar de apostar y girar la ruleta\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            // =================
            // APUESTA AL PLENO
            // =================
            case 1: {
                if (cantidad_plenos >= MAX_APUESTAS) {
                    printf("Ya alcanzaste el maximo de apuestas al pleno.\n");
                    break;
                }

                int dinero_disponible = *saldo - total_apostado;
                if (dinero_disponible <= 0) {
                    printf("No tenes dinero disponible para realizar otra apuesta.\n");
                    break;
                }

                int lectura;
                printf("Ingrese el numero al que quiere apostar (0 al 36): ");
                while ((lectura = scanf("%d", &numero_apostado)) != 1 ||
                       numero_apostado < 0 || numero_apostado > 36) {
                    if (lectura != 1) {
                        int caracter;
                        while ((caracter = getchar()) != '\n' && caracter != EOF) {}
                    }
                    printf("Entrada invalida. Ingrese un numero del 0 al 36: ");
                }

                printf("Ingrese el monto a apostar: ");
                scanf("%d", &monto_pleno);
                while (monto_pleno <= 0 || total_apostado + monto_pleno > *saldo) {
                    printf("Monto invalido. Dinero disponible para apostar: $%d\n",
                           *saldo - total_apostado);
                    printf("Ingrese nuevamente el monto: ");
                    scanf("%d", &monto_pleno);
                }

                // SE GUARDA LA APUESTA EN LOS ARREGLOS
                numeros_apostados[cantidad_plenos] = numero_apostado;
                montos_pleno[cantidad_plenos] = monto_pleno;
                cantidad_plenos++;

                *hubo_apuestas = 1;
                total_apostado = total_apostado + monto_pleno;

                printf("Apuesta al numero %d por $%d registrada.\n",
                       numero_apostado, monto_pleno);
                break;
            }

            // =================
            // APUESTA AL COLOR
            // =================
            case 2: {
                if (cantidad_colores >= MAX_APUESTAS) {
                    printf("Ya alcanzaste el maximo de apuestas al color.\n");
                    break;
                }

                int dinero_disponible = *saldo - total_apostado;
                if (dinero_disponible <= 0) {
                    printf("No tenes dinero disponible para realizar otra apuesta.\n");
                    break;
                }

                printf("Ingrese el color a apostar (rojo o negro): ");
                scanf("%9s", color_apostado);
                for (int i = 0; color_apostado[i] != '\0'; i++)
                    color_apostado[i] = (char)tolower((unsigned char)color_apostado[i]);

                while (strcmp(color_apostado, "rojo") != 0 &&
                       strcmp(color_apostado, "negro") != 0) {
                    printf("Color invalido. Ingrese rojo o negro: ");
                    scanf("%9s", color_apostado);
                    for (int i = 0; color_apostado[i] != '\0'; i++)
                        color_apostado[i] = (char)tolower((unsigned char)color_apostado[i]);
                }

                printf("Ingrese el monto a apostar: ");
                scanf("%d", &monto_color);
                while (monto_color <= 0 || total_apostado + monto_color > *saldo) {
                    printf("Monto invalido. Dinero disponible para apostar: $%d\n",
                           *saldo - total_apostado);
                    printf("Ingrese nuevamente el monto: ");
                    scanf("%d", &monto_color);
                }

                strcpy(colores_apostados[cantidad_colores], color_apostado);
                montos_color[cantidad_colores] = monto_color;
                cantidad_colores++;

                *hubo_apuestas = 1;
                total_apostado = total_apostado + monto_color;

                printf("Apuesta al color %s por $%d registrada.\n",
                       color_apostado, monto_color);
                break;
            }

            // ====================
            // APUESTA A LA PARIDAD
            // ====================
            case 3: {
                if (cantidad_paridades >= MAX_APUESTAS) {
                    printf("Ya alcanzaste el maximo de apuestas a la paridad.\n");
                    break;
                }

                int dinero_disponible = *saldo - total_apostado;
                if (dinero_disponible <= 0) {
                    printf("No tenes dinero disponible para realizar otra apuesta.\n");
                    break;
                }

                printf("Ingrese la paridad a apostar (par o impar): ");
                scanf("%9s", paridad_apostada);
                for (int i = 0; paridad_apostada[i] != '\0'; i++)
                    paridad_apostada[i] = (char)tolower((unsigned char)paridad_apostada[i]);

                while (strcmp(paridad_apostada, "par") != 0 &&
                       strcmp(paridad_apostada, "impar") != 0) {
                    printf("Paridad invalida. Ingrese par o impar: ");
                    scanf("%9s", paridad_apostada);
                    for (int i = 0; paridad_apostada[i] != '\0'; i++)
                        paridad_apostada[i] = (char)tolower((unsigned char)paridad_apostada[i]);
                }

                printf("Ingrese el monto a apostar: ");
                scanf("%d", &monto_paridad);
                while (monto_paridad <= 0 || total_apostado + monto_paridad > *saldo) {
                    printf("Monto invalido. Dinero disponible para apostar: $%d\n",
                           *saldo - total_apostado);
                    printf("Ingrese nuevamente el monto: ");
                    scanf("%d", &monto_paridad);
                }

                strcpy(paridades_apostadas[cantidad_paridades], paridad_apostada);
                montos_paridad[cantidad_paridades] = monto_paridad;
                cantidad_paridades++;

                *hubo_apuestas = 1;
                total_apostado = total_apostado + monto_paridad;

                printf("Apuesta a %s por $%d registrada.\n",
                       paridad_apostada, monto_paridad);
                break;
            }

            // ===================
            // APUESTA A LA DOCENA
            // ===================
            case 4: {
                if (cantidad_docenas >= MAX_APUESTAS) {
                    printf("Ya alcanzaste el maximo de apuestas a la docena.\n");
                    break;
                }

                int dinero_disponible = *saldo - total_apostado;
                if (dinero_disponible <= 0) {
                    printf("No tenes dinero disponible para realizar otra apuesta.\n");
                    break;
                }

                printf("Ingrese la docena a apostar (primera, segunda o tercera): ");
                scanf("%19s", docena_apostada);
                for (int i = 0; docena_apostada[i] != '\0'; i++)
                    docena_apostada[i] = (char)tolower((unsigned char)docena_apostada[i]);

                while (strcmp(docena_apostada, "primera") != 0 &&
                       strcmp(docena_apostada, "segunda") != 0 &&
                       strcmp(docena_apostada, "tercera") != 0) {
                    printf("Docena invalida. Ingrese primera, segunda o tercera: ");
                    scanf("%19s", docena_apostada);
                    for (int i = 0; docena_apostada[i] != '\0'; i++)
                        docena_apostada[i] = (char)tolower((unsigned char)docena_apostada[i]);
                }

                printf("Ingrese el monto a apostar: ");
                scanf("%d", &monto_docena);
                while (monto_docena <= 0 || total_apostado + monto_docena > *saldo) {
                    printf("Monto invalido. Dinero disponible para apostar: $%d\n",
                           *saldo - total_apostado);
                    printf("Ingrese nuevamente el monto: ");
                    scanf("%d", &monto_docena);
                }

                strcpy(docenas_apostadas[cantidad_docenas], docena_apostada);
                montos_docena[cantidad_docenas] = monto_docena;
                cantidad_docenas++;

                *hubo_apuestas = 1;
                total_apostado = total_apostado + monto_docena;

                printf("Apuesta a la docena %s por $%d registrada.\n",
                       docena_apostada, monto_docena);
                break;
            }

            case 5:
                if (total_apostado == 0) {
                    numero_salido = girar_ruleta();
                    printf("\nLa ruleta giro y salio el numero: %d\n", numero_salido);
                    printf("Todavia no realizaste apuestas. No cuenta como ronda.\n");
                    opcion = 0;
                } else {
                    printf("Terminaste de apostar. Girando la ruleta...\n");
                }
                break;

            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 5);

    // SI NO HUBO APUESTAS, NO SE RESUELVE NI SE CUENTA LA RONDA
    if (total_apostado == 0) {
        return;
    }

    // ========================================================
    //                GIRO DE LA RULETA
    // ========================================================

    numero_salido = girar_ruleta();
    color_salido = color_numero(numero_salido);
    paridad_salida = paridad(numero_salido);
    docena_salida = docena_numero(numero_salido);

    printf("\n====================================\n");
    printf("       RESULTADO DE LA RULETA\n");
    printf("====================================\n");
    printf("Numero: %d\n", numero_salido);
    printf("Color: %s\n", color_salido);
    printf("Paridad: %s\n", paridad_salida);
    printf("Docena: %s\n", docena_salida);

    // =======================================================
    //           RESOLUCION DE LAS APUESTAS
    // =======================================================

    // SE RECORREN TODAS LAS APUESTAS AL PLENO
    for (int i = 0; i < cantidad_plenos; i++) {
        if (verificar_pleno(numero_salido, numeros_apostados[i]) == 1) {
            int ganancia = montos_pleno[i] * 35;
            int total_recibido = montos_pleno[i] + ganancia;
            premio_total = premio_total + total_recibido;
            printf("\nGanaste el pleno al numero %d!\n", numeros_apostados[i]);
            printf("Ganancia: $%d | Total recibido: $%d\n", ganancia, total_recibido);
        } else {
            printf("\nPerdiste el pleno al numero %d.\n", numeros_apostados[i]);
        }
    }

    // SE RECORREN TODAS LAS APUESTAS AL COLOR
    for (int i = 0; i < cantidad_colores; i++) {
        if (verificar_color(color_salido, colores_apostados[i]) == 1) {
            int ganancia = montos_color[i];
            int total_recibido = montos_color[i] + ganancia;
            premio_total = premio_total + total_recibido;
            printf("\nGanaste la apuesta al color %s!\n", colores_apostados[i]);
            printf("Ganancia: $%d | Total recibido: $%d\n", ganancia, total_recibido);
        } else {
            printf("\nPerdiste la apuesta al color %s.\n", colores_apostados[i]);
        }
    }

    // SE RECORREN TODAS LAS APUESTAS A LA PARIDAD
    for (int i = 0; i < cantidad_paridades; i++) {
        if (verificar_paridad(paridad_salida, paridades_apostadas[i]) == 1) {
            int ganancia = montos_paridad[i];
            int total_recibido = montos_paridad[i] + ganancia;
            premio_total = premio_total + total_recibido;
            printf("\nGanaste la apuesta a %s!\n", paridades_apostadas[i]);
            printf("Ganancia: $%d | Total recibido: $%d\n", ganancia, total_recibido);
        } else {
            printf("\nPerdiste la apuesta a %s.\n", paridades_apostadas[i]);
        }
    }

    // SE RECORREN TODAS LAS APUESTAS A LA DOCENA
    for (int i = 0; i < cantidad_docenas; i++) {
        if (verificar_docena(docena_salida, docenas_apostadas[i]) == 1) {
            int ganancia = montos_docena[i] * 2;
            int total_recibido = montos_docena[i] + ganancia;
            premio_total = premio_total + total_recibido;
            printf("\nGanaste la apuesta a la docena %s!\n", docenas_apostadas[i]);
            printf("Ganancia: $%d | Total recibido: $%d\n", ganancia, total_recibido);
        } else {
            printf("\nPerdiste la apuesta a la docena %s.\n", docenas_apostadas[i]);
        }
    }

    // ==================================================
    //              ACTUALIZAR SALDO
    // ==================================================

    *saldo = *saldo - total_apostado + premio_total;

    printf("\n====================================\n");
    printf("          RESUMEN DE LA RONDA\n");
    printf("====================================\n");
    printf("Total apostado: $%d\n", total_apostado);
    printf("Premios recibidos: $%d\n", premio_total);
    printf("Saldo final: $%d\n", *saldo);
}
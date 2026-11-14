#include <stdio.h>
#include "gestion_juego.h"
#include "ronda.h"

void iniciar_juego(void) {
    char nombre[50];
    int saldo = 5000;
    int opcion = 0;
    int rondas = 0;
    int continuar = 1;

    printf("Ingrese su nombre completo: ");
    scanf(" %49[^\n]", nombre);

    printf("\nBienvenido a la ruleta, %s!\n", nombre);

    printf("\n--- REGLAS DEL JUEGO ---\n");
    printf("1. La ruleta tiene numeros del 0 al 36.\n");
    printf("2. Puede apostar a pleno, color, paridad y/o docena.\n");
    printf("3. Su saldo inicial es de $5000.\n");
    printf("4. No puede apostar mas dinero del que tiene.\n");
    printf("5. Puede jugar un maximo de 5 rondas.\n");

    printf("\nSu saldo actual es: $%d\n", saldo);

    do {
        printf("\n--- MENU DE LA RULETA ---\n");
        printf("1. Jugar\n");
        printf("2. Ver ranking\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                int hubo_apuestas = 0;
                jugar_ronda(&saldo, &hubo_apuestas);

                if (hubo_apuestas == 1) {
                    rondas++;
                }

                printf("\nRonda %d de 5\n", rondas);
                printf("Saldo actual: $%d\n", saldo);

                if (rondas < 5 && saldo > 0) {
                    printf("\nDesea continuar jugando? (1 = Si, 0 = No): ");
                    scanf("%d", &continuar);
                } else {
                    continuar = 0;
                    printf("\nFin de la partida.\n");
                }
                break;
            }

            case 2:
                printf("\n--- RANKING ---\n");
                printf("El ranking todavia no esta implementado.\n");
                break;

            case 3:
                printf("Hasta luego, vuelva pronto!\n");
                continuar = 0;
                break;

            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (rondas < 5 && continuar == 1 && saldo > 0 && opcion != 3);

    printf("\n--- RESUMEN DE LA PARTIDA ---\n");
    printf("Jugador: %s\n", nombre);
    printf("Rondas jugadas: %d\n", rondas);
    printf("Saldo final: $%d\n", saldo);
    printf("Gracias por jugar!\n");
}
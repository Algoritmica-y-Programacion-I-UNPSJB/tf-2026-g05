#include <SDL3/SDL.h>
#include <stdio.h>
#include "interfaz.h"
#include "ronda.h"
#include <math.h>
#include <string.h>
#include "ranking.h"
#include "gestion_juego.h" // funciones para administrar la partida

// ==========================================================
//              FUNCIONES AUXILIARES DE DIBUJO
// ==========================================================

void dibujar_texto(SDL_Renderer *renderer, const char *texto, int x, int y);

// DIBUJA TEXTO NORMAL CON EL TAMANO Y COLOR DEFINIDOS PARA LA INTERFAZ
void dibujar_texto(SDL_Renderer *renderer, const char *texto, int x, int y) {
    // TEXTO MAS GRANDE PARA UNA INTERFAZ MAS MODERNA
    const float escala = 1.35f;

    SDL_SetRenderScale(renderer, escala, escala);
    SDL_SetRenderDrawColor(renderer, 235, 205, 115, 255);
    SDL_RenderDebugText(renderer, x / escala, y / escala, texto);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

// DIBUJA TEXTO DESTACADO PARA TITULOS, RESULTADOS Y PREMIOS
void dibujar_texto_destacado(SDL_Renderer *renderer,
    const char *texto, int x, int y) {

    const float escala = 1.8f;
    SDL_SetRenderScale(renderer, escala, escala);
    SDL_SetRenderDrawColor(renderer, 255, 225, 145, 255);


    SDL_RenderDebugText(renderer, x / escala, y / escala, texto);
    SDL_RenderDebugText(renderer, (x + 1) / escala, y / escala, texto);

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}


// DIBUJA UN CIRCULO RELLENO
void dibujar_circulo(SDL_Renderer *renderer, int centro_x, int centro_y, int radio) {

    for (int y = -radio; y <= radio; y++) {

        int ancho = (int)sqrtf(
            radio * radio - y * y
        );

        SDL_RenderLine(
            renderer,
            centro_x - ancho,
            centro_y + y,
            centro_x + ancho,
            centro_y + y
        );
    }
}

// DIBUJA EL CONTORNO DE UN CIRCULO
void dibujar_circunferencia(SDL_Renderer *renderer, int centro_x, int centro_y, int radio) {

    for (int angulo = 0; angulo < 360; angulo++) {

        float radianes = angulo * M_PI / 180.0f;

        int x = centro_x + cosf(radianes) * radio;
        int y = centro_y + sinf(radianes) * radio;

        SDL_RenderPoint(renderer, x, y);
    }
}

// DIBUJA UNA FICHA CON SU COLOR Y VALOR
void dibujar_ficha(SDL_Renderer *renderer, int x, int y, int radio, Uint8 rojo, Uint8 verde, Uint8 azul, int valor) {
    // Borde dorado exterior
    SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
    dibujar_circulo(renderer, x, y, radio);

    // Interior de la ficha
    SDL_SetRenderDrawColor(renderer, rojo, verde, azul, 255);
    dibujar_circulo(renderer, x, y, radio - 4);

    // Centro dorado
    SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
    dibujar_circulo(renderer, x, y, radio / 3);

    // Valor de la ficha
    char texto_valor[10];
    sprintf(texto_valor, "%d", valor);

    dibujar_texto(renderer, texto_valor, x - 8, y - 4);
}

// CONSTRUYE Y DIBUJA UN SECTOR DE LA RULETA
void dibujar_sector(SDL_Renderer *renderer, int centro_x, int centro_y, float radio_interior, float radio_exterior, float angulo_inicio, float angulo_fin,
    SDL_FColor color) {

    SDL_Vertex vertices[10];
    int indices[24];

    for (int i = 0; i <= 4; i++) {

        float angulo = angulo_inicio + (angulo_fin - angulo_inicio) * i / 4.0f;

        vertices[i * 2].position = (SDL_FPoint){
            centro_x + cosf(angulo) * radio_exterior,
            centro_y + sinf(angulo) * radio_exterior
        };

        vertices[i * 2 + 1].position = (SDL_FPoint){
            centro_x + cosf(angulo) * radio_interior,
            centro_y + sinf(angulo) * radio_interior
        };

        vertices[i * 2].color = color;
        vertices[i * 2 + 1].color = color;

        vertices[i * 2].tex_coord = (SDL_FPoint){0, 0};
        vertices[i * 2 + 1].tex_coord = (SDL_FPoint){0, 0};
    }

    for (int i = 0; i < 4; i++) {

        int v = i * 2;
        int t = i * 6;

        indices[t] = v;
        indices[t + 1] = v + 1;
        indices[t + 2] = v + 2;

        indices[t + 3] = v + 1;
        indices[t + 4] = v + 3;
        indices[t + 5] = v + 2;
    }

    SDL_RenderGeometry(renderer, NULL, vertices, 10, indices, 24);
}


// ======================================================
//             DIBUJO DE LA RULETA GRAFICA
// ======================================================

int numeros_ruleta[37] = {
    0, 32, 15, 19, 4, 21, 2, 25, 17, 34,
    6, 27, 13, 36, 11, 30, 8, 23, 10, 5,
    24, 16, 33, 1, 20, 14, 31, 9, 22, 18,
    29, 7, 28, 12, 35, 3, 26
};

// INDICA SI EL NUMERO PERTENECE A LOS NUMEROS ROJOS
int es_rojo(int numero) {

    switch (numero) {
        case 1: case 3: case 5: case 7:
        case 9: case 12: case 14: case 16:
        case 18: case 19: case 21: case 23:
        case 25: case 27: case 30: case 32:
        case 34: case 36:
        return 1;

        default:
        return 0;
    }
}

// DIBUJA LOS 37 SECTORES CON SUS COLORES CORRESPONDIENTES
void dibujar_sectores(SDL_Renderer *renderer) {

    float angulo_sector = (2.0f * M_PI) / 37.0f;

    for (int i = 0; i < 37; i++) {

        SDL_FColor color;

        if (numeros_ruleta[i] == 0) {
            color = (SDL_FColor){0.0f, 0.5f, 0.1f, 1.0f};
        }
        else if (es_rojo(numeros_ruleta[i])) {
            color = (SDL_FColor){0.7f, 0.05f, 0.08f, 1.0f};
        }
        else {
            color = (SDL_FColor){0.08f, 0.08f, 0.08f, 1.0f};
        }

        float inicio = -M_PI / 2 + i * angulo_sector;
        float fin = inicio + angulo_sector;

        dibujar_sector(renderer, 360, 450, 125, 220, inicio, fin, color);
    }
}

// UBICA LOS NUMEROS EN EL CENTRO DE CADA SECTOR
void dibujar_numeros(SDL_Renderer *renderer) {
    float angulo_sector = (2.0f * M_PI) / 37.0f;

    for (int i = 0; i < 37; i++) {
        float angulo = -M_PI / 2 + (i + 0.5f) * angulo_sector;

        int x = 360 + cosf(angulo) * 165;
        int y = 450 + sinf(angulo) * 165;

        char texto[4];
        sprintf(texto, "%d", numeros_ruleta[i]);

        // NUMEROS SIN CIRCULOS DE FONDO, DIRECTAMENTE SOBRE LOS SECTORES
        SDL_SetRenderDrawColor(renderer, 250, 245, 230, 255);
        SDL_RenderDebugText(renderer, (float)(x - 5), (float)(y - 5), texto);
    }
}

// DIBUJA LAS LINEAS QUE SEPARAN LOS SECTORES
void dibujar_divisiones(SDL_Renderer *renderer) {
    float angulo_sector = (2.0f * M_PI) / 37.0f;

    SDL_SetRenderDrawColor(renderer, 70, 55, 30, 255);

    for (int i = 0; i < 37; i++) {
        float angulo = -M_PI / 2 + i * angulo_sector;

        int x1 = 360 + cosf(angulo) * 125;
        int y1 = 450 + sinf(angulo) * 125;

        int x2 = 360 + cosf(angulo) * 220;
        int y2 = 450 + sinf(angulo) * 220;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

// DIBUJA EL ADORNO CENTRAL DE LA RULETA
void dibujar_roseta(SDL_Renderer *renderer) {

    int centro_x = 360;
    int centro_y = 450;

    float radio_exterior = 28;
    float radio_interior = 8;

    // OCHO RAYOS DORADOS
    SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);

    for (int i = 0; i < 8; i++) {

        float angulo = (2.0f * M_PI * i) / 8.0f;

        int x1 = centro_x + cosf(angulo) * radio_interior;
        int y1 = centro_y + sinf(angulo) * radio_interior;

        int x2 = centro_x + cosf(angulo) * radio_exterior;
        int y2 = centro_y + sinf(angulo) * radio_exterior;

        SDL_RenderLine(renderer, x1, y1, x2, y2);

        // CIRCULOS EN LAS PUNTAS
        dibujar_circulo(renderer, x2, y2, 3);
    }

    // CENTRO DORADO
    SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
    dibujar_circulo(renderer, centro_x, centro_y, 7);

    // CENTRO NEGRO
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    dibujar_circulo(renderer, centro_x, centro_y, 3);
}


// ==========================================================
//          ELEMENTOS DEL TABLERO Y LOS BOTONES
// ==========================================================

// DIBUJA EL FONDO Y EL NUMERO DE CADA CASILLA
void dibujar_casilla(SDL_Renderer* renderer, int x, int y,
    int ancho, int alto, int numero) {
    SDL_FColor color;
    if (numero == 0) color = (SDL_FColor){0.0f, 0.42f, 0.08f, 1.0f};
    else if (es_rojo(numero)) color = (SDL_FColor){0.58f, 0.025f, 0.045f, 1.0f};
    else color = (SDL_FColor){0.035f, 0.035f, 0.035f, 1.0f};

    SDL_FRect casilla = {x, y, ancho, alto};
    SDL_SetRenderDrawColor(renderer, (Uint8)(color.r * 255),
        (Uint8)(color.g * 255), (Uint8)(color.b * 255), 255);
    SDL_RenderFillRect(renderer, &casilla);

    char texto[4];
    sprintf(texto, "%d", numero);
    dibujar_texto(renderer, texto, x + ancho / 2 - 5, y + alto / 2 - 5);
}

// DIBUJA TEXTO CENTRADO DENTRO DE UNA CASILLA
void dibujar_casilla_texto(SDL_Renderer *renderer,
    int x, int y, int ancho, int alto, const char *texto) {
    int ancho_texto = strlen(texto) * 8;
    dibujar_texto(renderer, texto, x + (ancho - ancho_texto) / 2,
        y + (alto - 10) / 2);
}

// CONTORNO CON ESQUINAS REDONDEADAS
// DIBUJA EL BORDE DE UN RECTANGULO CON ESQUINAS REDONDEADAS
void dibujar_contorno_redondeado(SDL_Renderer *renderer,
    int x, int y, int ancho, int alto, int radio) {
    SDL_RenderLine(renderer, x + radio, y, x + ancho - radio, y);
    SDL_RenderLine(renderer, x + radio, y + alto, x + ancho - radio, y + alto);
    SDL_RenderLine(renderer, x, y + radio, x, y + alto - radio);
    SDL_RenderLine(renderer, x + ancho, y + radio, x + ancho, y + alto - radio);

    for (int angulo = 0; angulo <= 90; angulo++) {
        float rad = angulo * M_PI / 180.0f;
        int dx = (int)(cosf(rad) * radio);
        int dy = (int)(sinf(rad) * radio);
        SDL_RenderPoint(renderer, x + radio - dx, y + radio - dy);
        SDL_RenderPoint(renderer, x + ancho - radio + dx, y + radio - dy);
        SDL_RenderPoint(renderer, x + radio - dx, y + alto - radio + dy);
        SDL_RenderPoint(renderer, x + ancho - radio + dx, y + alto - radio + dy);
    }
}


// DIBUJA UN BOTON SIMPLE PARA LAS ACCIONES DE LA RONDA
// DIBUJA UN BOTON Y SU EFECTO CUANDO EL MOUSE ESTA ENCIMA
void dibujar_boton(SDL_Renderer *renderer, int x, int y,
    int ancho, int alto, const char *texto, int mouse_x, int mouse_y) {

    int hover = mouse_x >= x && mouse_x <= x + ancho &&
    mouse_y >= y && mouse_y <= y + alto;

    // SOMBRA
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_FRect sombra = { x + 4, y + 5, ancho, alto };
    SDL_RenderFillRect(renderer, &sombra);

    // PLACA NEGRA CON REFLEJO DORADO AL PASAR EL MOUSE
    SDL_FRect boton = { x, y, ancho, alto };
    SDL_SetRenderDrawColor(renderer,
        hover ? 75 : 20, hover ? 58 : 18, hover ? 25 : 16, 255);
    SDL_RenderFillRect(renderer, &boton);

    // BORDE DOBLE DORADO
    SDL_SetRenderDrawColor(renderer, 105, 75, 28, 255);
    dibujar_contorno_redondeado(renderer, x, y, ancho, alto, 13);

    SDL_SetRenderDrawColor(renderer,
        hover ? 255 : 212, hover ? 225 : 175, hover ? 130 : 55, 255);
    dibujar_contorno_redondeado(renderer, x + 2, y + 2,
        ancho - 4, alto - 4, 11);

    // TEXTO CENTRADO
    int ancho_texto = (int)strlen(texto) * 8;
    SDL_SetRenderDrawColor(renderer, 255, 225, 155, 255);
    dibujar_texto(renderer, texto,
        x + (ancho - ancho_texto) / 2, y + alto / 2 - 7);
}

// DIBUJA UNA FICHA PEQUENA SOBRE EL SECTOR DONDE SE APOSTO
// DIBUJA LA MARCA VISUAL DE UNA FICHA APOSTADA
void dibujar_marca_apuesta(SDL_Renderer *renderer, int x, int y, int valor) {

    Uint8 rojo, verde, azul;

    if (valor == 1) {
        rojo = 20; verde = 105; azul = 245;
    } else if (valor == 5) {
        rojo = 20; verde = 175; azul = 75;
    } else {
        rojo = 220; verde = 30; azul = 45;
    }

    // SOMBRA PARA SEPARAR LA FICHA DEL TABLERO
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    dibujar_circulo(renderer, x + 2, y + 3, 19);

    // ARO EXTERIOR BLANCO Y BORDE DE COLOR
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    dibujar_circulo(renderer, x, y, 18);

    SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
    dibujar_circulo(renderer, x, y, 15);

    // CUERPO DE LA FICHA
    SDL_SetRenderDrawColor(renderer, rojo, verde, azul, 255);
    dibujar_circulo(renderer, x, y, 13);

    // CENTRO OSCURO PARA DAR CONTRASTE AL VALOR
    SDL_SetRenderDrawColor(renderer, 10, 18, 28, 255);
    dibujar_circulo(renderer, x, y, 8);

    // VALOR BLANCO Y BIEN VISIBLE
    char texto_valor[8];
    snprintf(texto_valor, sizeof(texto_valor), "%d", valor);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, (float)(x - (valor == 25 ? 7 : 4)),
        (float)(y - 4), texto_valor);

    // BRILLO SUPERIOR
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 210);
    dibujar_circunferencia(renderer, x, y, 16);
}

// ========================================================
//             INICIO Y CONFIGURACION DE SDL3
// ========================================================

void iniciar_interfaz(void) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error al iniciar SDL3: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *ventana = SDL_CreateWindow(
        "ROYAL ROULETTE",
        1400,
        850,
        0
    );
    if (ventana == NULL) {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(ventana, NULL);

    if (renderer == NULL) {
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    // CARGAR TEXTURA DE MADERA

    SDL_Surface *superficie_madera = SDL_LoadBMP("recursos/madera.bmp");

    if (superficie_madera == NULL) {
        printf("Error al cargar madera.bmp: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    SDL_Texture *textura_madera =
    SDL_CreateTextureFromSurface(renderer, superficie_madera);

    SDL_DestroySurface(superficie_madera);

    // CARGAR TEXTURA DEL PAÑO VERDE

    SDL_Surface *superficie_pano = SDL_LoadBMP("recursos/paño_verde.bmp");

    if (superficie_pano == NULL) {
        printf("Error al cargar paño_verde.bmp: %s\n", SDL_GetError());
        SDL_DestroyTexture(textura_madera);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    SDL_Texture *textura_pano =
    SDL_CreateTextureFromSurface(renderer, superficie_pano);

    SDL_DestroySurface(superficie_pano);

    // CARGAR FICHA DE VALOR 1

    SDL_Surface *superficie_ficha_1 =
    SDL_LoadBMP("recursos/ficha_1.bmp");

    if (superficie_ficha_1 == NULL) {
        printf("Error al cargar ficha_1.bmp: %s\n", SDL_GetError());
        SDL_DestroyTexture(textura_pano);
        SDL_DestroyTexture(textura_madera);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    SDL_Texture *textura_ficha_1 =
    SDL_CreateTextureFromSurface(renderer, superficie_ficha_1);

    SDL_DestroySurface(superficie_ficha_1);

    if (textura_ficha_1 == NULL) {
        printf("Error al crear textura de ficha_1: %s\n", SDL_GetError());
        SDL_DestroyTexture(textura_pano);
        SDL_DestroyTexture(textura_madera);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    // CARGAR FICHA DE VALOR 5

    SDL_Surface *superficie_ficha_5 =
    SDL_LoadBMP("recursos/ficha_5.bmp");

    SDL_Texture *textura_ficha_5 = NULL;

    if (superficie_ficha_5 == NULL) {
        printf("Error al cargar ficha_5.bmp: %s\n", SDL_GetError());
    } else {
        textura_ficha_5 =
        SDL_CreateTextureFromSurface(renderer, superficie_ficha_5);

        SDL_DestroySurface(superficie_ficha_5);
    }


    // CARGAR FICHA DE VALOR 25

    SDL_Surface *superficie_ficha_25 =
    SDL_LoadBMP("recursos/ficha_25.bmp");

    SDL_Texture *textura_ficha_25 = NULL;

    if (superficie_ficha_25 == NULL) {
        printf("Error al cargar ficha_25.bmp: %s\n", SDL_GetError());
    } else {
        textura_ficha_25 =
        SDL_CreateTextureFromSurface(renderer, superficie_ficha_25);

        SDL_DestroySurface(superficie_ficha_25);
    }

    if (textura_pano == NULL) {
        printf("Error al crear textura del paño: %s\n", SDL_GetError());
        SDL_DestroyTexture(textura_madera);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    if (textura_madera == NULL) {
        printf("Error al crear textura: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return;
    }

    // ======================================================
    //       VARIABLES Y ESTADO PROPIO DE LA INTERFAZ
    // ======================================================
    int ejecutando = 1;
    SDL_Event evento;

    // ESTADO DE LAS PANTALLAS
    int pantalla_actual = 0;
    int ranking_desde_menu = 0;
    // 0 = ingreso del nombre
    // 1 = reglas
    // 2 = juego
    // 3 = ranking
    // 4 = menu de bienvenida

    // NOMBRE DEL JUGADOR
    char nombre_jugador[50] = "";

    SDL_StartTextInput(ventana);


    // ESTADO GENERAL DEL JUEGO
    EstadoJuego juego;

    // Inicializa el estado general del juego
    iniciar_partida(&juego, "");


    // VARIABLES PROPIAS DE LA INTERFAZ
    int valor_ficha_seleccionada = 1;
    int mouse_x_actual = 0;
    int mouse_y_actual = 0;

    int saldo_mostrado = 1000;

    // POSICIONES DE LAS FICHAS COLOCADAS SOBRE EL TABLERO
    int marcas_x[MAX_APUESTAS * 4];
    int marcas_y[MAX_APUESTAS * 4];
    int marcas_valor[MAX_APUESTAS * 4];
    int cantidad_marcas = 0;

    char mensaje[120] = "Seleccione una ficha y haga sus apuestas.";

    // ESTADO DE LA ANIMACION DE LA BOLA
    int bola_x = 360;
    int bola_y = 245;
    float angulo_bola = -M_PI / 2.0f;
    float angulo_inicio_giro = 0.0f;
    float angulo_destino_giro = -M_PI / 2.0f;
    const float radio_bola = 205.0f;
    int animacion_giro = 0;
    Uint64 inicio_giro = 0;
    const Uint64 duracion_giro = 4200;

    // ======================================================
    //              BUCLE PRINCIPAL DEL PROGRAMA
    // ======================================================
    while (ejecutando) {

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                ejecutando = 0;
            }

            // --------------------------------------------------
            // ENTRADA DEL NOMBRE Y EVENTOS DEL TECLADO
            // --------------------------------------------------
            // CAPTURAR EL NOMBRE DEL JUGADOR
            if (pantalla_actual == 0 && evento.type == SDL_EVENT_TEXT_INPUT) {

                size_t longitud_actual = strlen(nombre_jugador);
                size_t longitud_nueva = strlen(evento.text.text);

                if (longitud_actual + longitud_nueva < sizeof(nombre_jugador)) {
                    strcat(nombre_jugador, evento.text.text);
                }
            }

            // BORRAR EL ULTIMO CARACTER CON BACKSPACE
            if (pantalla_actual == 0 &&
                evento.type == SDL_EVENT_KEY_DOWN &&
                evento.key.key == SDLK_BACKSPACE) {

                size_t longitud = strlen(nombre_jugador);

                if (longitud > 0) {
                    longitud--;

                    while (longitud > 0 &&
                        ((unsigned char)nombre_jugador[longitud] & 0xC0) == 0x80) {
                        longitud--;
                    }

                    nombre_jugador[longitud] = '\0';
                }
            }

            // --------------------------------------------------
            // EVENTOS DEL MOUSE Y ACCIONES DE LOS BOTONES
            // --------------------------------------------------
            // ACTUALIZAR LA POSICION DEL MOUSE PARA EL EFECTO HOVER
            if (evento.type == SDL_EVENT_MOUSE_MOTION) {
                mouse_x_actual = (int)evento.motion.x;
                mouse_y_actual = (int)evento.motion.y;
            }

            if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                evento.button.button == SDL_BUTTON_LEFT) {

                int mx = (int)evento.button.x;
                int my = (int)evento.button.y;
                mouse_x_actual = mx;
                mouse_y_actual = my;

                // ======================================
                //       BOTONES DE BIENVENIDA
                // ======================================

                // BOTON CONTINUAR DESPUES DE INGRESAR EL NOMBRE
                if (pantalla_actual == 0 &&
                    mx >= 500 && mx <= 900 && my >= 480 && my <= 535) {

                    if (strlen(nombre_jugador) > 0) {
                        pantalla_actual = 4;
                        SDL_StopTextInput(ventana);
                    }
                }

                // OPCIONES DEL MENU DE BIENVENIDA
                // ELSE IF evita que el mismo clic de CONTINUAR
                // tambien active el boton JUGAR.
                else if (pantalla_actual == 4) {

                    // BOTON JUGAR
                    if (mx >= 500 && mx <= 900 && my >= 480 && my <= 535) {

                        iniciar_partida(&juego, nombre_jugador);
                        valor_ficha_seleccionada = 1;
                        cantidad_marcas = 0;
                        saldo_mostrado = juego.saldo;
                        pantalla_actual = 2;

                        snprintf(mensaje, sizeof(mensaje), "Realice sus apuestas.");
                    }
                    // BOTON REGLAS
                    else if (mx >= 500 && mx <= 900 && my >= 560 && my <= 615) {
                        pantalla_actual = 1;
                    }
                    // BOTON SALIR
                    else if (mx >= 500 && mx <= 900 && my >= 640 && my <= 695) {
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(ventana);
                        SDL_Quit();
                        return;
                    }
                }

                // BOTON VOLVER AL MENU DESDE REGLAS
                if (pantalla_actual == 1 &&
                    mx >= 500 && mx <= 900 &&
                    my >= 760 && my <= 815) {

                    pantalla_actual = 4;
                }


                // --------------------------------------------------
                // ACCIONES DEL JUEGO: APUESTAS Y CONTROL DE RONDA
                // La interfaz interpreta en que zona se hizo clic y
                // llama a las funciones de gestion_juego.c.
                // --------------------------------------------------
                if (pantalla_actual == 2 && !juego.partida_terminada && !animacion_giro) {

                    // SELECCION DE FICHAS
                    if (!juego.ronda_resuelta) {
                        if (mx >= 610 && mx <= 665 && my >= 295 && my <= 350) {
                            valor_ficha_seleccionada = 1;
                            snprintf(mensaje, sizeof(mensaje), "Ficha seleccionada: $1");
                        }
                        else if (mx >= 610 && mx <= 665 && my >= 360 && my <= 415) {
                            valor_ficha_seleccionada = 5;
                            snprintf(mensaje, sizeof(mensaje), "Ficha seleccionada: $5");
                        }
                        else if (mx >= 610 && mx <= 665 && my >= 425 && my <= 480) {
                            valor_ficha_seleccionada = 25;
                            snprintf(mensaje, sizeof(mensaje), "Ficha seleccionada: $25");
                        }
                        // APUESTA AL CERO
                        else if (mx >= 690 && mx < 735 && my >= 270 && my < 450) {
                            if (apostar_pleno(&juego, 0, valor_ficha_seleccionada)) {
                                if (cantidad_marcas < MAX_APUESTAS * 4) {
                                    marcas_x[cantidad_marcas] = 712;
                                    marcas_y[cantidad_marcas] = 410;
                                    marcas_valor[cantidad_marcas++] = valor_ficha_seleccionada;
                                }
                                snprintf(mensaje, sizeof(mensaje), "Apuesta al 0: $%d", valor_ficha_seleccionada);
                            } else {
                                snprintf(mensaje, sizeof(mensaje), "Saldo insuficiente o limite de apuestas.");
                            }
                        }


                        // APUESTAS A LOS NUMEROS DEL 1 AL 36
                        else if (mx >= 735 && mx < 1335 && my >= 270 && my < 450) {
                            int columna = (mx - 735) / 50;
                            int fila = (my - 270) / 60;
                            int numero = columna * 3 + (3 - fila);

                            if (apostar_pleno(&juego, numero,
                                valor_ficha_seleccionada)) {
                                if (cantidad_marcas < MAX_APUESTAS * 4) {
                                    // CENTRO EXACTO DE LA CASILLA
                                    marcas_x[cantidad_marcas] = 735 + columna * 50 + 25;
                                    marcas_y[cantidad_marcas] = 270 + fila * 60 + 30;
                                    marcas_valor[cantidad_marcas++] = valor_ficha_seleccionada;
                                }
                                snprintf(mensaje, sizeof(mensaje), "Apuesta al %d: $%d", numero, valor_ficha_seleccionada);
                            } else {
                                snprintf(mensaje, sizeof(mensaje), "Saldo insuficiente o limite de apuestas.");
                            }
                        }


                        // APUESTAS A DOCENAS
                        else if (mx >= 735 && mx < 1335 && my >= 455 && my < 495) {
                            const char *docena = (mx < 935) ? "primera" :
                            (mx < 1135) ? "segunda" : "tercera";

                            if (apostar_docena(&juego, docena,
                                valor_ficha_seleccionada)) {
                                if (cantidad_marcas < MAX_APUESTAS * 4) {
                                    int indice_docena = (mx < 935) ? 0 : (mx < 1135) ? 1 : 2;
                                    marcas_x[cantidad_marcas] = 835 + indice_docena * 200;
                                    marcas_y[cantidad_marcas] = 475;
                                    marcas_valor[cantidad_marcas++] = valor_ficha_seleccionada;
                                }
                                snprintf(mensaje, sizeof(mensaje), "Apuesta a la %s docena: $%d", docena, valor_ficha_seleccionada);
                            } else {
                                snprintf(mensaje, sizeof(mensaje), "Saldo insuficiente o limite de apuestas.");
                            }
                        }
                        // APUESTAS DE PARIDAD Y COLOR
                        else if (mx >= 735 && mx < 1335 && my >= 500 && my < 540) {
                            int sector = (mx - 735) / 150;
                            int aceptada = 0;

                            if (sector == 0) {
                                aceptada = apostar_paridad(&juego, "par", valor_ficha_seleccionada);
                            } else if (sector == 1) {
                                aceptada = apostar_color(&juego, "rojo", valor_ficha_seleccionada);
                            } else if (sector == 2) {
                                aceptada = apostar_color(&juego, "negro", valor_ficha_seleccionada);
                            } else {
                                aceptada = apostar_paridad(&juego, "impar", valor_ficha_seleccionada);
                            }

                            if (aceptada) {
                                if (cantidad_marcas < MAX_APUESTAS * 4) {
                                    marcas_x[cantidad_marcas] = 810 + sector * 150;
                                    marcas_y[cantidad_marcas] = 520;
                                    marcas_valor[cantidad_marcas++] = valor_ficha_seleccionada;
                                }
                                snprintf(mensaje, sizeof(mensaje), "Apuesta registrada: $%d", valor_ficha_seleccionada);
                            } else {
                                snprintf(mensaje, sizeof(mensaje), "Saldo insuficiente o limite de apuestas.");
                            }
                        }

                        // BOTON GIRAR RULETA

                        else if (mx >= 1120 && mx <= 1320 && my >= 650 && my <= 695) {
                            if (juego.ronda.total_apostado > 0) {
                                if (resolver_ronda_juego(&juego)) {

                                    // La logica ya resolvio la ronda y guardo el numero ganador.
                                    // La interfaz busca su posicion para animar la bola.
                                    // BUSCAR EL ANGULO DEL NUMERO GANADOR
                                    int indice_ganador = 0;

                                    for (int i = 0; i < 37; i++) {
                                        if (numeros_ruleta[i] == juego.resultado.numero_salido) {
                                            indice_ganador = i;
                                            break;
                                        }
                                    }

                                    float paso_sector = (2.0f * M_PI) / 37.0f;
                                    angulo_inicio_giro = angulo_bola;
                                    angulo_destino_giro = -M_PI / 2.0f +
                                    (indice_ganador + 0.5f) * paso_sector;

                                    // RECORRER VARIAS VUELTAS Y FRENAR EN EL GANADOR
                                    float diferencia = angulo_destino_giro - angulo_inicio_giro;
                                    while (diferencia < 0.0f) diferencia += 2.0f * M_PI;
                                    while (diferencia >= 2.0f * M_PI) diferencia -= 2.0f * M_PI;

                                    angulo_destino_giro = angulo_inicio_giro +
                                    7.0f * 2.0f * M_PI + diferencia;

                                    inicio_giro = SDL_GetTicks();
                                    animacion_giro = 1;

                                    snprintf(mensaje, sizeof(mensaje),
                                        "La bola esta girando...");
                                } else {
                                    snprintf(mensaje, sizeof(mensaje), "No se pudo resolver la ronda.");
                                }
                            } else {
                                snprintf(mensaje, sizeof(mensaje), "Primero debe realizar al menos una apuesta.");
                            }
                        }
                    }

                    // BOTON PARA INICIAR LA SIGUIENTE RONDA
                    else if (mx >= 1120 && mx <= 1320 && my >= 650 && my <= 695) {

                        if (avanzar_ronda_juego(&juego)) {

                            // Limpiamos las fichas visuales de la ronda anterior
                            cantidad_marcas = 0;

                            snprintf(mensaje, sizeof(mensaje), "Ronda %d de 5. Realice sus apuestas.", juego.numero_ronda);
                        } else {
                            snprintf(mensaje, sizeof(mensaje),
                                "No se pudo iniciar la siguiente ronda.");
                        }
                    }
                }



                // BOTON VER TOP 5
                if (pantalla_actual == 2 && juego.partida_terminada && juego.ronda_resuelta && !animacion_giro && mx >= 1120 && mx <= 1320 && my >= 650 && my <= 695) {

                    Jugador jugador;

                    snprintf(jugador.nombre, sizeof(jugador.nombre), "%s", nombre_jugador);

                    jugador.puntaje = juego.saldo;

                    guardar_puntaje(jugador);

                    ranking_desde_menu = 0;
                    pantalla_actual = 3;
                }


                // BOTON VOLVER AL MENU DESDE EL TOP 5
                if (pantalla_actual == 3 &&
                    mx >= 500 && mx <= 900 &&
                    my >= 700 && my <= 755) {

                    pantalla_actual = 0;

                    // Si venimos del final de una partida,
                    // limpiar el nombre para un nuevo jugador.
                    if (!ranking_desde_menu) {
                        nombre_jugador[0] = '\0';
                    }

                    ranking_desde_menu = 0;

                    SDL_StartTextInput(ventana);
                }


            }
        }

        // ======================================================
        //             ACTUALIZACION DE LA ANIMACION
        // ======================================================
        // ANIMACION: RAPIDA AL PRINCIPIO Y CADA VEZ MAS LENTA
        if (animacion_giro) {
            float progreso = (float)(SDL_GetTicks() - inicio_giro) /
            (float)duracion_giro;

            if (progreso >= 1.0f) {
                progreso = 1.0f;
                animacion_giro = 0;

                // El saldo ya fue actualizado por la logica del juego.
                // Aqui solo actualizamos el valor que se muestra en pantalla.
                saldo_mostrado = juego.saldo;

                angulo_bola = angulo_destino_giro;
                snprintf(mensaje, sizeof(mensaje), "Salio %d %s. Premio: $%d", juego.resultado.numero_salido, juego.resultado.color_salido, juego.resultado.premio_total);
            } else {
                // EASING DE FRENADO SUAVE
                float avance = 1.0f - powf(1.0f - progreso, 4.0f);
                angulo_bola = angulo_inicio_giro +
                (angulo_destino_giro - angulo_inicio_giro) * avance;
            }
        }

        bola_x = 360 + (int)(cosf(angulo_bola) * radio_bola);
        bola_y = 450 + (int)(sinf(angulo_bola) * radio_bola);

        // ======================================================
        //          DIBUJO Y ACTUALIZACION DE LA PANTALLA
        // ======================================================
        SDL_SetRenderDrawColor(renderer, 0, 100, 50, 255);
        SDL_RenderClear(renderer);

        // FONDO DE MADERA

        SDL_FRect fondo_madera = {
            0, 0, 1400, 850
        };

        SDL_RenderTexture(renderer, textura_madera, NULL, &fondo_madera);

        // ------------------------------------------------------
        // PANTALLA PRINCIPAL DEL JUEGO
        // ------------------------------------------------------
        if (pantalla_actual == 2) {

            // PAÑO VERDE DE LA MESA

            SDL_FRect fondo_pano = {
                60, 100, 1280, 700
            };

            SDL_RenderTexture(renderer, textura_pano, NULL, &fondo_pano);



            SDL_FRect mesa = { 60, 100, 1280, 700 };

            SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
            SDL_RenderRect(renderer, &mesa);

            // =====================================================
            //                  TABLERO DE APUESTAS
            // =====================================================

            int ancho_casilla = 50;
            int alto_casilla = 60;
            int inicio_x = 735;
            int inicio_y = 270;

            // LINEAS BLANCAS DEL TABLERO
            SDL_SetRenderDrawColor(renderer, 235, 235, 225, 255);

            // CASILLA VERDE DEL CERO
            dibujar_casilla(renderer, 690, 270, 45, 180, 0);

            // NUMEROS DEL 1 AL 36
            for (int fila = 0; fila < 3; fila++) {
                for (int columna = 0; columna < 12; columna++) {
                    int numero = columna * 3 + (3 - fila);
                    int x = inicio_x + columna * ancho_casilla;
                    int y = inicio_y + fila * alto_casilla;
                    dibujar_casilla(renderer, x, y, ancho_casilla, alto_casilla, numero);
                }
            }

            // CONTORNO Y DIVISIONES DE LOS NUMEROS
            SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
            dibujar_contorno_redondeado(renderer, 690, 270, 650, 180, 12);
            SDL_RenderLine(renderer, 735, 270, 735, 450);
            for (int fila = 1; fila < 3; fila++)
            SDL_RenderLine(renderer, 735, 270 + fila * alto_casilla, 1340, 270 + fila * alto_casilla);
            for (int columna = 1; columna < 12; columna++)
            SDL_RenderLine(renderer, inicio_x + columna * ancho_casilla, 270,
                inicio_x + columna * ancho_casilla, 450);

            // APUESTAS POR DOCENA
            dibujar_casilla_texto(renderer, 735, 455, 200, 40, "1.ª DOCENA");
            dibujar_casilla_texto(renderer, 935, 455, 200, 40, "2.ª DOCENA");
            dibujar_casilla_texto(renderer, 1135, 455, 200, 40, "3.ª DOCENA");
            SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
            dibujar_contorno_redondeado(renderer, 735, 455, 600, 40, 9);
            SDL_RenderLine(renderer, 935, 455, 935, 495);
            SDL_RenderLine(renderer, 1135, 455, 1135, 495);

            // APUESTAS DE PARIDAD Y COLOR
            dibujar_casilla_texto(renderer, 735, 500, 150, 40, "PAR");
            dibujar_casilla_texto(renderer, 885, 500, 150, 40, "ROJO");
            dibujar_casilla_texto(renderer, 1035, 500, 150, 40, "NEGRO");
            dibujar_casilla_texto(renderer, 1185, 500, 150, 40, "IMPAR");
            SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
            dibujar_contorno_redondeado(renderer, 735, 500, 600, 40, 9);
            SDL_RenderLine(renderer, 885, 500, 885, 540);
            SDL_RenderLine(renderer, 1035, 500, 1035, 540);
            SDL_RenderLine(renderer, 1185, 500, 1185, 540);


            // =====================================================
            //                 FICHAS DE APUESTA
            // =====================================================

            // FICHA AZUL - VALOR 1
            SDL_FRect ficha_azul = {606, 291, 63, 63};
            SDL_RenderTexture(renderer, textura_ficha_1, NULL, &ficha_azul);

            // FICHA VERDE - VALOR 5
            SDL_FRect ficha_verde = {606, 356, 63, 63};
            SDL_RenderTexture(renderer, textura_ficha_5, NULL, &ficha_verde);

            // FICHA ROJA - VALOR 25
            SDL_FRect ficha_roja = {606, 421, 63, 63};
            SDL_RenderTexture(renderer, textura_ficha_25, NULL, &ficha_roja);

            // RESALTAR LA FICHA SELECCIONADA
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (valor_ficha_seleccionada == 1) {
                SDL_FRect seleccion = {603, 288, 69, 69};
                SDL_RenderRect(renderer, &seleccion);
            } else if (valor_ficha_seleccionada == 5) {
                SDL_FRect seleccion = {603, 353, 69, 69};
                SDL_RenderRect(renderer, &seleccion);
            } else {
                SDL_FRect seleccion = {603, 418, 69, 69};
                SDL_RenderRect(renderer, &seleccion);
            }

            // MOSTRAR LAS FICHAS APOSTADAS SOBRE EL TABLERO
            for (int i = 0; i < cantidad_marcas; i++) {

                // CONTAR SOLO LAS FICHAS QUE ESTAN EN EL MISMO SECTOR
                int repeticion = 0;
                for (int j = 0; j < i; j++) {
                    if (marcas_x[j] == marcas_x[i] && marcas_y[j] == marcas_y[i]) {
                        repeticion++;
                    }
                }

                // DESPLAZAMIENTO PEQUENO Y LOCAL PARA QUE NO SE PISEN
                int desplazamiento_x = (repeticion % 3 - 1) * 7;
                int desplazamiento_y = ((repeticion / 3) % 3 - 1) * 7;

                dibujar_marca_apuesta(renderer,
                    marcas_x[i] + desplazamiento_x,
                    marcas_y[i] + desplazamiento_y,
                    marcas_valor[i]);
            }


            // Sombra exterior
            SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
            dibujar_circulo(renderer, 360, 453, 238);

            // Base metalica dorada
            SDL_SetRenderDrawColor(renderer, 120, 82, 25, 255);
            dibujar_circulo(renderer, 360, 450, 237);

            // Reflejo dorado
            SDL_SetRenderDrawColor(renderer, 230, 190, 85, 255);
            dibujar_circulo(renderer, 360, 450, 234);

            // Superficie oscura interior del aro
            SDL_SetRenderDrawColor(renderer, 35, 25, 15, 255);
            dibujar_circulo(renderer, 360, 450, 230);


            // BASE INTERIOR OSCURA
            SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
            dibujar_circulo(renderer, 360, 450, 220);


            // SECTORES DE LA RULETA
            dibujar_sectores(renderer);

            dibujar_numeros(renderer);

            dibujar_divisiones(renderer);


            // ARO DORADO EXTERIOR DE LOS SECTORES
            SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
            dibujar_circunferencia(renderer, 360, 450, 220);

            // ARO DORADO INTERIOR DE LOS SECTORES
            SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
            dibujar_circunferencia(renderer, 360, 450, 125);


            // ANILLO DORADO INTERIOR
            SDL_SetRenderDrawColor(renderer, 115, 80, 25, 255);
            dibujar_circulo(renderer, 360, 450, 125);

            // REFLEJO DORADO
            SDL_SetRenderDrawColor(renderer, 225, 185, 75, 255);
            dibujar_circulo(renderer, 360, 450, 121);

            // CENTRO OSCURO DEL ANILLO
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            dibujar_circulo(renderer, 360, 450, 117);

            // BOLA DE LA RULETA: ESTELA DE MOVIMIENTO DURANTE EL GIRO
            if (animacion_giro) {
                for (int estela = 4; estela >= 1; estela--) {
                    float angulo_estela = angulo_bola - estela * 0.055f;
                    int estela_x = 360 + (int)(cosf(angulo_estela) * radio_bola);
                    int estela_y = 450 + (int)(sinf(angulo_estela) * radio_bola);

                    SDL_SetRenderDrawColor(renderer, 210, 225, 240,
                        (Uint8)(35 + (4 - estela) * 28));
                    dibujar_circulo(renderer, estela_x, estela_y, 7);
                }
            }

            // SOMBRA DE LA BOLA
            SDL_SetRenderDrawColor(renderer, 5, 5, 5, 190);
            dibujar_circulo(renderer, bola_x, bola_y + 3, 11);

            // BOLA BLANCA
            SDL_SetRenderDrawColor(renderer, 250, 250, 245, 255);
            dibujar_circulo(renderer, bola_x, bola_y, 9);

            // BRILLO DE LA BOLA
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            dibujar_circulo(renderer, bola_x - 3, bola_y - 3, 3);


            // CENTRO METALICO DE LA RULETA

            // Base exterior dorada
            SDL_SetRenderDrawColor(renderer, 100, 65, 20, 255);
            dibujar_circulo(renderer, 360, 450, 55);

            // Borde dorado brillante
            SDL_SetRenderDrawColor(renderer, 235, 195, 90, 255);
            dibujar_circulo(renderer, 360, 450, 50);

            // Anillo de bronce
            SDL_SetRenderDrawColor(renderer, 125, 85, 35, 255);
            dibujar_circulo(renderer, 360, 450, 44);

            // Centro oscuro
            SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
            dibujar_circulo(renderer, 360, 450, 38);

            // Nucleo dorado
            SDL_SetRenderDrawColor(renderer, 190, 145, 55, 255);
            dibujar_circulo(renderer, 360, 450, 12);

            // Centro negro
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
            dibujar_circulo(renderer, 360, 450, 7);

            dibujar_roseta(renderer);

            // PANEL SUPERIOR
            SDL_FRect panel = { 60, 20, 1280, 55 };

            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderFillRect(renderer, &panel);

            dibujar_texto(renderer, "            /ROYAL ROULETTE/             ", 420, 35);



            SDL_SetRenderDrawColor(renderer, 35, 210, 190, 255);
            SDL_RenderRect(renderer, &panel);

            // INFORMACION DE LA PARTIDA
            char texto_saldo[60];
            char texto_ronda[40];
            char texto_apostado[60];

            char texto_jugador[70];

            snprintf(texto_jugador, sizeof(texto_jugador),
                "JUGADOR: %s", nombre_jugador);

            dibujar_texto(renderer, texto_jugador, 850, 35);

            snprintf(texto_saldo, sizeof(texto_saldo), "SALDO: $%d", saldo_mostrado);

            snprintf(texto_ronda, sizeof(texto_ronda), "RONDA: %d / 5", juego.numero_ronda);

            snprintf(texto_apostado, sizeof(texto_apostado), "APOSTADO: $%d", juego.ronda.total_apostado);

            dibujar_texto(renderer, texto_saldo, 720, 585);
            dibujar_texto(renderer, texto_ronda, 920, 585);
            dibujar_texto(renderer, texto_apostado, 1080, 585);
            dibujar_texto(renderer, mensaje, 720, 620);

            if (animacion_giro) {
                dibujar_boton(renderer, 1120, 650, 200, 45, "GIRANDO...", mouse_x_actual, mouse_y_actual);
            } else if (!juego.ronda_resuelta && !juego.partida_terminada) {
                dibujar_boton(renderer, 1120, 650, 200, 45, "GIRAR RULETA", mouse_x_actual, mouse_y_actual);
            } else if (juego.ronda_resuelta && !juego.partida_terminada) {
                dibujar_boton(renderer, 1120, 650, 200, 45, "SIGUIENTE RONDA", mouse_x_actual, mouse_y_actual);
            } else {
                dibujar_boton(renderer, 1120, 650, 200, 45, "FIN DE PARTIDA", mouse_x_actual, mouse_y_actual);
            }

            if (juego.ronda_resuelta && !animacion_giro) {
                char resultado_texto[100];
                snprintf(resultado_texto, sizeof(resultado_texto),
                    "RESULTADO: %d - %s", juego.resultado.numero_salido, juego.resultado.color_salido);
                dibujar_texto_destacado(renderer, resultado_texto, 720, 675);
                char premio_texto[80];
                snprintf(premio_texto, sizeof(premio_texto),
                    "PREMIO: $%d   SALDO FINAL: $%d", juego.resultado.premio_total, juego.resultado.saldo_final);
                dibujar_texto_destacado(renderer, premio_texto, 720, 710);
            }
        }


        // =====================================================
        //              PANTALLA DE INGRESO
        // =====================================================

        if (pantalla_actual == 0) {

            dibujar_texto_destacado(renderer, "ROYAL ROULETTE", 490, 180);
            dibujar_texto(renderer, "INGRESE SU NOMBRE:", 540, 320);

            // CAMPO DONDE SE MUESTRA EL NOMBRE INGRESADO
            SDL_FRect campo_nombre = { 450, 360, 500, 55 };
            SDL_SetRenderDrawColor(renderer, 212, 175, 55, 255);
            SDL_RenderRect(renderer, &campo_nombre);
            dibujar_texto(renderer, nombre_jugador, 470, 380);

            dibujar_boton(renderer, 500, 480, 400, 55,
                "CONTINUAR", mouse_x_actual, mouse_y_actual);
        }

        // =====================================================
        //          MENU DE BIENVENIDA Y SALUDO
        // =====================================================

        if (pantalla_actual == 4) {

            char saludo[100];
            snprintf(saludo, sizeof(saludo), "BIENVENIDO, %s", nombre_jugador);

            // Título principal
            dibujar_texto_destacado(renderer, "ROYAL ROULETTE", 600, 250);

            // Saludo personalizado
            dibujar_texto_destacado(renderer, saludo, 600, 350);

            // Botones del menú
            dibujar_boton(renderer, 500, 480, 400, 55,
                "JUGAR", mouse_x_actual, mouse_y_actual);

            dibujar_boton(renderer, 500, 560, 400, 55,
                "REGLAS DEL JUEGO", mouse_x_actual, mouse_y_actual);

            dibujar_boton(renderer, 500, 640, 400, 55,
                "SALIR", mouse_x_actual, mouse_y_actual);
        }

        // =====================================================
        //                 PANTALLA DE REGLAS
        // =====================================================


        if (pantalla_actual == 1) {

            dibujar_texto_destacado(renderer, "REGLAS DEL JUEGO", 500, 90);

            // TIPO DE RULETA
            dibujar_texto(renderer, "TIPO DE RULETA:", 350, 170);
            dibujar_texto(renderer, "Ruleta europea con 37 numeros: del 0 al 36.", 350, 205);
            dibujar_texto(renderer, "El 0 es verde y los demas numeros son rojos o negros.", 350, 240);

            // TIPOS DE APUESTAS
            dibujar_texto(renderer, "TIPOS DE APUESTAS Y PREMIOS:", 350, 300);

            dibujar_texto(renderer, "PLENO: acertar un numero exacto. Premio: 35 a 1.", 350, 340);
            dibujar_texto(renderer, "COLOR: apostar a rojo o negro. Premio: 1 a 1.", 350, 375);
            dibujar_texto(renderer, "PARIDAD: apostar a par o impar. Premio: 1 a 1.", 350, 410);
            dibujar_texto(renderer, "DOCENAS: apostar a una de las tres docenas. Premio: 2 a 1.", 350, 445);

            // SALDO INICIAL
            dibujar_texto(renderer, "SALDO INICIAL: $1000", 350, 505);

            // DESARROLLO DE LA PARTIDA
            dibujar_texto(renderer, "DESARROLLO DE LA PARTIDA:", 350, 555);
            dibujar_texto(renderer, "Selecciona tus fichas y realiza tus apuestas.", 350, 590);
            dibujar_texto(renderer, "La ruleta gira y determina el numero ganador.", 350, 625);

            // FINALIZACION
            dibujar_texto(renderer, "La partida tiene un maximo de 5 rondas.", 350, 660);
            dibujar_texto(renderer, "Al finalizar, se muestra el ranking de jugadores.", 350, 690);

            dibujar_boton(renderer, 500, 760, 400, 55,
                "VOLVER AL MENU", mouse_x_actual, mouse_y_actual);
        }




        // =====================================================
        //                 PANTALLA TOP 5
        // =====================================================

        if (pantalla_actual == 3) {

            Jugador jugadores[TOP_RANKING];
            int cantidad = cargar_top_ranking(jugadores);

            dibujar_texto_destacado(renderer, "TOP 5 ROYAL ROULETTE", 450, 120);

            if (cantidad == 0) {
                dibujar_texto(renderer, "TODAVIA NO HAY PUNTAJES.", 500, 300);
            }

            for (int i = 0; i < cantidad; i++) {

                char linea[100];

                snprintf(linea, sizeof(linea), "%d. %s - $%d",
                    i + 1, jugadores[i].nombre, jugadores[i].puntaje);

                dibujar_texto(renderer, linea, 450, 220 + i * 65);
            }

            dibujar_boton(renderer, 500, 700, 400, 55,
                "VOLVER AL MENU", mouse_x_actual, mouse_y_actual);

        }



        // PRESENTA EN PANTALLA TODO LO QUE SE DIBUJO
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // ======================================================
    //             LIBERAR RECURSOS DE SDL3
    // ======================================================
    // LIBERAR TEXTURAS DE LAS FICHAS
    SDL_DestroyTexture(textura_ficha_1);
    SDL_DestroyTexture(textura_ficha_5);
    SDL_DestroyTexture(textura_ficha_25);

    // LIBERAR TEXTURAS DE LA MESA
    SDL_DestroyTexture(textura_pano);
    SDL_DestroyTexture(textura_madera);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

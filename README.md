# Trabajo Integrador Final 2026 - Grupo 05

Juego de ruleta europea (0 a 36) para un jugador contra la banca, con interfaz gráfica desarrollada en SDL3.

## Dependencias
- SDL3
- Biblioteca matemática estándar (`math.h`)

## Estructura
- `main.c`: punto de entrada
- `interfaz.c` / `interfaz.h`: interfaz gráfica
- `ruleta.c` / `ruleta.h`: lógica de la ruleta
- `ronda.c` / `ronda.h`: apuestas y resolución de rondas
- `apuesta.c` / `apuesta.h`: verificaciones de apuestas
- `ranking.c` / `ranking.h`: almacenamiento y visualización del ranking
- `gestion_juego.c` / `gestion_juego.h`: gestión del juego por consola
- `recursos/`: imágenes BMP utilizadas por la interfaz

El archivo `ranking.dat` se genera durante la ejecución y no se incluye en el repositorio.

# 🎰 ROYAL ROULETTE
### Trabajo Integrador Final 2026 – Grupo 05

**Juego:** Ruleta Europea  
**Modalidad:** 1 jugador contra la banca  
**Interfaz:** Gráfica, desarrollada con SDL3  
**Lenguaje:** C

---

## 1. Integrantes y aportes

- **Franco Sarabia** (@francosarabia67)

- **Leonel Luna** (@deku-tdf)

## 2. Reglas y alcance del juego

Royal Roulette es una ruleta europea en la que participa un jugador contra la banca.

El juego cuenta con números del 0 al 36 y un saldo inicial de $5000.

Se pueden realizar cuatro tipos de apuestas:

- **Pleno:** apostar a un número exacto. Premio de 35 a 1.
- **Color:** apostar a rojo o negro. Premio de 1 a 1.
- **Paridad:** apostar a par o impar. Premio de 1 a 1.
- **Docena:** apostar a la primera, segunda o tercera docena. Premio de 2 a 1.

**Condiciones de la partida:**
- Se pueden jugar hasta 5 rondas.
- No se puede apostar más dinero del disponible.
- Se pueden realizar varias apuestas antes de girar la ruleta.
- El saldo se actualiza al finalizar cada ronda.
- Al terminar la partida se muestra el ranking de los mejores 5 puntajes.

## 3. Instrucciones para compilar y ejecutar

### Requisitos

- Compilador de C (GCC).
- Biblioteca SDL3 instalada.
- Herramienta `pkg-config` configurada para SDL3.

### Compilación

Desde la carpeta principal del proyecto, ejecutar:

```bash
gcc main.c interfaz.c ronda.c ruleta.c apuesta.c ranking.c gestion_juego.c -o royal_roulette $(pkg-config --cflags --libs sdl3) -lm
```

### Ejecución

En Windows, abrir una terminal en la carpeta del proyecto y ejecutar:

```bash
./royal_roulette.exe
```

Es necesario iniciar el programa desde la carpeta principal para que pueda cargar correctamente las imágenes ubicadas en `recursos/`.

### Controles del juego

- Escribir el nombre del jugador utilizando el teclado.
- Utilizar el mouse para seleccionar las opciones del menú.
- Seleccionar una ficha de $1, $5 o $25.
- Hacer clic en el tablero para realizar las apuestas.
- Presionar el botón "GIRAR RULETA" para obtener el resultado.
- Utilizar "SIGUIENTE RONDA" para continuar la partida.

  
## 4. Organización general del programa

El proyecto está dividido en distintos módulos para separar las responsabilidades.

| Archivo | Función |
|---|---|
| `main.c` | Inicializa la semilla aleatoria e inicia la interfaz gráfica. |
| `interfaz.c / interfaz.h` | Gestiona las pantallas, los eventos del mouse, los gráficos y la interacción con el jugador. |
| `ruleta.c / ruleta.h` | Genera el número aleatorio y determina su color, paridad y docena. |
| `ronda.c / ronda.h` | Registra las apuestas, administra los datos de cada ronda y resuelve los resultados. |
| `apuesta.c / apuesta.h` | Comprueba si las apuestas realizadas coinciden con el resultado obtenido. |
| `ranking.c / ranking.h` | Guarda, ordena y muestra los mejores puntajes. |
| `gestion_juego.c / gestion_juego.h` | Contiene la implementación de la gestión del juego en consola. |

### Organización de los datos

Se utilizan estructuras para almacenar la información de las apuestas y los resultados de cada ronda.

- `Ronda`: almacena las apuestas realizadas, sus montos y el total apostado.
- `ResultadoRonda`: almacena el número obtenido, su color, paridad, docena, premio y saldo final.
- `Jugador`: almacena el nombre del jugador y su puntaje.

El ranking se guarda en un archivo binario llamado `ranking.dat`, que se genera al guardar los primeros puntajes.

### Recorrido general del programa

1. Se inicia el programa desde `main.c`.
2. Se abre la interfaz gráfica.
3. El jugador ingresa su nombre y puede consultar las reglas.
4. Selecciona las fichas y realiza sus apuestas.
5. La ruleta gira y se obtiene un resultado.
6. Se verifican las apuestas y se actualiza el saldo.
7. Se permite continuar hasta alcanzar el límite de rondas o finalizar la partida.
8. Se guarda el puntaje y se muestra el Top 5.

## 5. Relación entre la interfaz gráfica y la lógica del juego

La interfaz gráfica fue desarrollada utilizando la biblioteca SDL3.

El módulo `interfaz.c` se encarga de mostrar los elementos visuales y capturar las acciones del jugador mediante eventos del mouse y del teclado.

La interfaz se comunica con la lógica del juego a través de las funciones:

- `iniciar_ronda()`: prepara una nueva ronda.
- `registrar_apuesta_pleno()`: registra apuestas a números.
- `registrar_apuesta_color()`: registra apuestas a colores.
- `registrar_apuesta_paridad()`: registra apuestas a paridad.
- `registrar_apuesta_docena()`: registra apuestas a docenas.
- `resolver_ronda()`: realiza el giro, resuelve las apuestas y actualiza el saldo.
- `guardar_puntaje()` y `cargar_top_ranking()`: gestionan el ranking.

La parte gráfica incluye la ruleta, el tablero de apuestas, las fichas, los botones, las animaciones y las distintas pantallas del juego.

Los recursos gráficos se encuentran en la carpeta `recursos/` y se cargan mediante SDL3.

**Recursos gráficos y uso de IA:** [Indicar cuáles fueron elaborados por el grupo, cuáles se obtuvieron de otras fuentes y si se utilizaron herramientas de inteligencia artificial para generarlos o modificarlos].

---

## 6. Recursos del proyecto

La carpeta `recursos/` contiene las imágenes utilizadas por la interfaz:

- `madera.bmp`
- `paño_verde.bmp`
- `ficha_1.bmp`
- `ficha_5.bmp`
- `ficha_25.bmp`

Estos archivos son necesarios para visualizar correctamente la mesa y las fichas del juego.

---

**Trabajo Integrador Final 2026 – Grupo 05**  
**Royal Roulette**

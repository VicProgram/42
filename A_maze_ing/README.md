*Este proyecto ha sido creado como parte del currículo de 42 por vabad-ro y lvillarr.*

# A-Maze-ing

Generador y resolvedor de laberintos en terminal, con visualización ASCII a color y patrón especial "42" integrado.

---

## Descripción

**A-Maze-ing** es un programa de línea de comandos escrito en Python que genera laberintos aleatorios configurables, los resuelve automáticamente y los renderiza directamente en la terminal usando caracteres Unicode y colores ANSI.

El objetivo del proyecto es implementar un algoritmo de generación de laberintos desde cero, aplicando estructuras de datos propias, resolución por camino más corto y una capa visual interactiva. Como guiño a la escuela 42, el laberinto incorpora el número "42" grabado en su interior como patrón de celdas bloqueadas.

Características principales:

- Generación de laberintos perfectos (sin ciclos) o imperfectos (con bucles).
- Resolución automática mediante BFS (camino más corto).
- Visualización en terminal con más de 30 paletas de colores intercambiables.
- Soporte para semilla aleatoria reproducible (`SEED`).
- Exportación del laberinto y su solución a archivo `.txt` en formato hexadecimal.
- Menú interactivo para regenerar, mostrar/ocultar la solución y cambiar colores.

---

## Instrucciones

### Requisitos

- Python 3.10 o superior
- `make`

### Instalación y ejecución

```bash
# Clonar el repositorio
git clone <url-del-repositorio>
cd a-maze-ing

# Crear entorno virtual, instalar dependencias y ejecutar
make

# O de forma separada:
make install   # instala dependencias en el venv
make run       # ejecuta el programa
```

### Otros comandos disponibles

```bash
make lint        # ejecuta flake8 + mypy
make lint-strict # ejecuta mypy en modo --strict
make debug       # ejecuta con pdb
make clean       # elimina venv, __pycache__ y maze.txt
make re          # limpia y vuelve a compilar desde cero
```

### Ejecución manual (sin Makefile)

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 a-maze-ing.py config.txt
```

---

## Archivo de configuración (`config.txt`)

El programa se configura completamente a través del archivo `config.txt`, ubicado en la raíz del proyecto. Debe pasarse como argumento al ejecutar el programa.

### Formato

Cada línea sigue el formato `CLAVE=VALOR`. Las líneas que comienzan con `#` son comentarios y se ignoran. Las claves no pueden repetirse.

```
WIDTH=30
HEIGHT=20
ENTRY=2, 4
EXIT=29, 19
OUTPUT_FILE=maze.txt
PERFECT=True
#SEED=45
```

### Claves disponibles

| Clave | Tipo | Requerida | Descripción |
|---|---|---|---|
| `WIDTH` | entero (2–100) | Sí | Ancho del laberinto en celdas |
| `HEIGHT` | entero (2–100) | Sí | Alto del laberinto en celdas |
| `ENTRY` | `x, y` | Sí | Coordenadas de la celda de entrada |
| `EXIT` | `x, y` | Sí | Coordenadas de la celda de salida |
| `OUTPUT_FILE` | string `.txt` | Sí | Ruta del archivo de salida |
| `PERFECT` | `True` / `False` | Sí | Si es `True`, el laberinto no tendrá ciclos |
| `SEED` | entero | No | Semilla para resultados reproducibles |

### Restricciones

- `ENTRY` y `EXIT` deben estar dentro de los límites del laberinto.
- `ENTRY` y `EXIT` no pueden ser la misma celda.
- Ninguna de las dos puede caer sobre una celda del patrón "42".
- `OUTPUT_FILE` debe terminar en `.txt`.

---

## Formato del archivo de salida (`maze.txt`)

El archivo generado contiene la representación compacta del laberinto:

```
1DF...  ← una línea por fila (cada carácter es un hex que codifica las paredes)

x_entrada,y_entrada
x_salida,y_salida
NNESSW...  ← secuencia de movimientos de la solución (N/S/E/W)
```

Cada carácter hexadecimal codifica las cuatro paredes de una celda en 4 bits:

| Bit | Pared |
|---|---|
| 0 (LSB) | Norte (N) |
| 1 | Este (E) |
| 2 | Sur (S) |
| 3 | Oeste (W) |

`1` significa pared presente, `0` significa pared ausente.

---

## Algoritmo de generación: Recursive Backtracker (DFS con pila)

### ¿Qué algoritmo se usó?

Se implementó el algoritmo **Recursive Backtracker**, también conocido como **DFS iterativo con pila**. Partiendo de la celda de entrada, el algoritmo:

1. Marca la celda actual como visitada y la añade a la pila.
2. Elige aleatoriamente un vecino no visitado y no bloqueado (evitando celdas del patrón "42").
3. Rompe la pared entre la celda actual y el vecino elegido.
4. Avanza al vecino.
5. Si no hay vecinos disponibles, retrocede en la pila hasta encontrar una celda con vecinos válidos.
6. Repite hasta que todas las celdas accesibles hayan sido visitadas.

Si `PERFECT=False`, tras la generación se eliminan adicionalmente un porcentaje de paredes al azar para crear ciclos.

### ¿Por qué este algoritmo?

- **Laberintos con alto "factor de río":** los caminos generados son largos, con pocos cruces, lo que crea laberintos visualmente dramáticos y difíciles de resolver a ojo.
- **Implementación iterativa sencilla:** al usar una pila explícita en lugar de recursión real, se evita el desbordamiento de pila en laberintos grandes.
- **Control natural del punto de inicio:** el algoritmo arranca desde `ENTRY`, lo que garantiza que la entrada esté conectada al resto del laberinto desde el primer paso.
- **Compatibilidad con el patrón "42":** las celdas bloqueadas se excluyen de la generación de forma natural, sin romper la integridad del laberinto.

---

## Resolución: BFS (Búsqueda en anchura)

Una vez generado el laberinto, se resuelve usando **BFS** desde `ENTRY` hasta `EXIT`. BFS garantiza encontrar el camino más corto en número de pasos. La solución se marca visualmente en el laberinto y se exporta como secuencia de movimientos (`N`, `S`, `E`, `W`) en el archivo de salida.

---

## Partes reutilizables del código

El proyecto está diseñado de forma modular. Los siguientes componentes son completamente independientes y reutilizables en otros proyectos:

### `mazegen/generator.py` — Módulo `MazeGen`

La clase `MazeGen` encapsula toda la lógica de generación y resolución. Puede usarse de forma independiente pasando un objeto `Map`:

```python
from config_data import Map
from mazegen.generator import MazeGen

game_map = Map.from_file("config.txt")
gen = MazeGen(game_map=game_map, seed=42)
cells = gen.generate()
path, route = gen.solve()
```

### `cell.py` — Estructura de datos `Cell`

La clase `Cell` representa una celda con cuatro paredes binarias y metadatos de estado. Puede reutilizarse en cualquier sistema de cuadrícula 2D que necesite representar conectividad entre nodos.

### `file_writer.py` — Exportación hexadecimal

La función `write_maze_to_file` serializa cualquier cuadrícula de `Cell` a un formato de texto compacto. El esquema de codificación hex puede adaptarse fácilmente a otros formatos de salida.

### `printer.py` — Renderizador ASCII

La función `print_maze` genera una representación visual del laberinto como string. Es totalmente independiente del proceso de generación y acepta cualquier cuadrícula de `Cell` más un diccionario de paleta de colores.

### `colors.py` — Sistema de paletas ANSI

El módulo define más de 30 paletas de colores intercambiables como diccionarios simples. Se puede ampliar añadiendo nuevas entradas a la lista `PALETTES` sin modificar ningún otro archivo.

---

### Planificación

La planificación inicial contemplaba cuatro fases:

1. Diseño de la estructura de datos (`Cell`, `Map`) y lectura del archivo de configuración.
2. Implementación del algoritmo de generación y validación de la lógica de paredes.
3. Integración del solucionador BFS y exportación a archivo.
4. Capa visual: renderizador ASCII, paletas de colores y menú interactivo.

En la práctica, la integración del patrón "42" requirió más iteraciones de las previstas, ya que las celdas bloqueadas necesitaban ser ignoradas tanto en la generación como en la resolución sin romper la conectividad del laberinto.

### Qué funcionó bien

- La separación en módulos independientes facilitó el testing progresivo de cada parte.
- El uso de una semilla fija (`SEED`) permitió reproducir bugs concretos de forma determinista.
- El sistema de paletas como simples diccionarios resultó muy fácil de ampliar.
- El tipo `mypy` con flags estrictos ayudó a detectar errores de tipo antes de ejecutar.

### Qué se podría mejorar

- Añadir tests automáticos unitarios para `MazeGen` y `Map`.
- Implementar algoritmos alternativos de generación (Prim, Kruskal, Wilson) seleccionables desde el config.
- Añadir soporte para exportar el laberinto como imagen (PNG/SVG).
- Mejorar la gestión de errores cuando el laberinto es demasiado pequeño para el patrón "42".

### Herramientas utilizadas

- **Python 3.10+** — lenguaje principal
- **flake8** — linting de estilo (PEP 8)
- **mypy** — comprobación estática de tipos
- **make** — automatización del entorno y flujo de trabajo
- **git** — control de versiones
- **Claude (Anthropic)** — asistencia en la redacción de docstrings, revisión de estructura del README y generación de ideas para nombres de paletas de colores. No se usó IA para generar código de algoritmos ni lógica central del proyecto.

---

## Recursos

### Algoritmos de generación de laberintos

- [Maze Generation Algorithm — Wikipedia](https://en.wikipedia.org/wiki/Maze_generation_algorithm) — visión general comparativa de todos los algoritmos clásicos.
- [Think Labyrinth: Maze Algorithms (Walter D. Pullen)](https://www.astrolog.org/labyrnth/algrithm.htm) — referencia exhaustiva con análisis de propiedades de cada algoritmo.
- [Mazes for Programmers (Jamis Buck, The Pragmatic Bookshelf)](https://pragprog.com/titles/jbmaze/mazes-for-programmers/) — libro de referencia sobre implementación práctica de laberintos.
- [Buckblog: Maze Generation (Jamis Buck)](https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracker) — artículo específico sobre el Recursive Backtracker con animaciones.

### BFS y teoría de grafos

- [Breadth-First Search — Wikipedia](https://en.wikipedia.org/wiki/Breadth-first_search)
- [Python `collections.deque` — documentación oficial](https://docs.python.org/3/library/collections.html#collections.deque)

### Python y herramientas

- [Documentación oficial de Python 3](https://docs.python.org/3/)
- [mypy — documentación oficial](https://mypy.readthedocs.io/)
- [flake8 — documentación oficial](https://flake8.pycqa.org/)
- [Python `dataclasses` — documentación oficial](https://docs.python.org/3/library/dataclasses.html)

### Colores ANSI en terminal

- [ANSI escape codes — Wikipedia](https://en.wikipedia.org/wiki/ANSI_escape_code) — referencia de códigos de color y formato.

### Uso de IA

Se utilizó **Claude (Anthropic)** como asistente durante el proyecto para las siguientes tareas:

- Revisión y mejora de los docstrings de las funciones y clases.
- Generación de ideas para los nombres y esquemas de las paletas de colores.
- Estructuración y redacción de este README según las pautas del Capítulo VII.

No se utilizó IA para implementar ningún algoritmo, lógica de negocio ni estructura de datos del proyecto.
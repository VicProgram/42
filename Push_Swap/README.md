*Este proyecto ha sido creado como parte del currículo de 42 por vabad-ro,    lvillarr.*

# Push_swap

## Descripción

**Push_swap** es un proyecto algorítmico que consiste en ordenar una lista de enteros usando únicamente dos stacks (`a` y `b`) y un conjunto limitado de operaciones. El objetivo es encontrar la secuencia de instrucciones más corta posible para dejar el stack `a` ordenado de menor a mayor.

El reto no es solo ordenar, sino hacerlo de manera eficiente. Por eso el programa implementa cuatro estrategias distintas de ordenación y selecciona automáticamente la más adecuada según el **índice de desorden** de la entrada.

### Operaciones disponibles

| Operación | Descripción |
|-----------|-------------|
| `sa` / `sb` | Intercambia los dos primeros elementos del stack a / b |
| `ss` | `sa` y `sb` simultáneamente |
| `pa` / `pb` | Mueve el primer elemento de b a a / de a a b |
| `ra` / `rb` | Rota el stack a / b hacia arriba (el primero pasa al final) |
| `rr` | `ra` y `rb` simultáneamente |
| `rra` / `rrb` | Rota el stack a / b hacia abajo (el último pasa al principio) |
| `rrr` | `rra` y `rrb` simultáneamente |

---

## Instrucciones

### Compilación

```bash
make
```

### Uso

```bash
./push_swap [--simple | --medium | --complex | --adaptive] <lista de enteros>
```

**Selector de estrategia (opcional):**

- `--simple` — fuerza el algoritmo O(n²)
- `--medium` — fuerza el algoritmo O(n√n)
- `--complex` — fuerza el algoritmo O(n log n)
- `--adaptive` — selección automática según el índice de desorden (comportamiento por defecto)

**Modo benchmark:**

```bash
./push_swap --bench --adaptive 4 67 3 87 23 2> bench.txt | ./checker_linux 4 67 3 87 23
```

El modo `--bench` imprime a `stderr` el índice de desorden, la estrategia usada, el total de operaciones y el desglose por tipo.

### Ejemplos

```bash
# Ordenar 5 números con estrategia adaptativa
./push_swap 3 1 4 1 5

# Forzar algoritmo simple y verificar con checker
./push_swap --simple 5 4 3 2 1 | ./checker_linux 5 4 3 2 1

# Medir operaciones para 100 números aleatorios
shuf -i 0-999 -n 100 | tr '\n' ' ' | xargs ./push_swap | wc -l
```

### Manejo de errores

El programa imprime `Error` a `stderr` en los siguientes casos:
- Argumentos que no son enteros
- Enteros fuera del rango de `int`
- Números duplicados

---

## Algoritmos implementados

### Índice de desorden

Antes de elegir estrategia, el programa calcula el **índice de desorden**: una métrica entre 0 (completamente ordenado) y 1 (máximo desorden), basada en el número de pares inversamente ordenados sobre el total de pares posibles.

```
disorder = número de pares (i, j) con i < j y a[i] > a[j]  /  total de pares
```

---

### 1. Algoritmo simple — O(n²): Extracción del mínimo/máximo

**Estrategia:** en cada iteración se localiza el elemento mínimo (o máximo) del stack `a` y se desplaza a su posición correcta usando rotaciones y `push` hacia el stack `b`. Una vez todos los elementos están ordenados en `b`, se devuelven a `a` de golpe.

**Funcionamiento paso a paso:**
1. Se calcula la posición del mínimo actual en `a`.
2. Se rota `a` (con `ra` o `rra`, la que cueste menos movimientos) para traerlo al tope.
3. Se hace `pb` para enviarlo a `b`.
4. Se repite hasta que en `a` quede un solo elemento.
5. Se hacen `pa` sucesivos para devolver todos los elementos.

**Complejidad:** O(n²) operaciones en el modelo Push_swap, ya que cada extracción puede requerir hasta O(n) rotaciones.

**Cuándo se usa:** cuando el índice de desorden es muy bajo (< 0.2) en modo adaptativo, o al forzar `--simple`. Funciona bien para entradas pequeñas o casi ordenadas.

---

### 2. Algoritmo medio — O(n√n): Orden por chunks

**Estrategia:** se divide el rango de valores en bloques (*chunks*) de tamaño aproximado √n. Los elementos se envían a `b` chunk a chunk, y luego se recuperan en orden.

**Funcionamiento paso a paso:**
1. Se normalizan los valores (se asigna a cada número su rango de 0 a n-1).
2. Se divide ese rango en √n chunks consecutivos (por ejemplo, para n=100: chunks de ~10 valores cada uno).
3. Para cada chunk, se recorre `a` enviando a `b` con `pb` todos los elementos que pertenecen al chunk actual, rotando el stack para encontrarlos eficientemente.
4. Una vez que todos los elementos están en `b` (ordenados por chunks, con los de mayor valor al tope), se recuperan a `a` con `pa`, buscando siempre el máximo y rotando `b` para traerlo al frente.

**Complejidad:** O(n√n) operaciones. Hay √n chunks y por cada uno se hacen O(n) rotaciones para encontrar y mover los elementos del chunk. La fase de recuperación desde `b` también es O(n√n).

**Cuándo se usa:** cuando el índice de desorden es medio (0.2 ≤ desorden < 0.5) en modo adaptativo, o al forzar `--medium`. Equilibrio óptimo entre implementación sencilla y buen rendimiento para entradas medianas.

---

### 3. Algoritmo complejo — O(n log n): Radix sort LSD

**Estrategia:** adaptación del algoritmo Radix Sort (LSD — Least Significant Digit) al modelo de dos stacks. Se ordena bit a bit, de menos significativo a más significativo, usando las operaciones de Push_swap como único mecanismo de clasificación.

**Funcionamiento paso a paso:**
1. Se normalizan los valores a índices de 0 a n-1 (para trabajar con representación binaria limpia).
2. Para cada bit (de bit 0 a bit ⌈log₂n⌉):
   - Se recorre todo el stack `a`. Si el bit actual del elemento en el tope es `0`, se hace `pb` (va a `b`). Si es `1`, se hace `ra` (queda en `a`).
   - Al terminar el recorrido, se devuelven todos los elementos de `b` a `a` con `pa` sucesivos.
3. Tras procesar todos los bits, el stack `a` queda ordenado.

**Complejidad:** O(n log n) operaciones. Hay ⌈log₂n⌉ pasadas, y en cada pasada se realizan O(n) operaciones (una por elemento). El número de bits necesarios es log₂n, de ahí la cota.

**Cuándo se usa:** cuando el índice de desorden es alto (≥ 0.5) en modo adaptativo, o al forzar `--complex`. Es el algoritmo más eficiente para entradas grandes y muy desordenadas.

---

### 4. Algoritmo adaptativo

Combina los tres algoritmos anteriores seleccionando automáticamente el más adecuado según el índice de desorden calculado al inicio:

| Índice de desorden | Algoritmo usado | Complejidad |
|--------------------|-----------------|-------------|
| < 0.2 | Extracción del mínimo/máximo | O(n) — la entrada está casi ordenada, pocas correcciones |
| 0.2 ≤ desorden < 0.5 | Chunks (√n) | O(n√n) |
| ≥ 0.5 | Radix LSD | O(n log n) |

**Justificación de los umbrales:**
- El umbral 0.2 se eligió porque con bajo desorden, la mayoría de elementos ya están en posición y el algoritmo de extracción del mínimo solo necesita unos pocos movimientos, comportándose efectivamente en O(n).
- El umbral 0.5 separa el desorden "moderado" del "alto". Por encima de este punto, las permutaciones son suficientemente aleatorias como para que Radix, con su complejidad garantizada O(n log n) independientemente de la distribución, sea siempre preferible al enfoque de chunks.

---

### 5. Contribuciones del equipo
Aunque ambos integrantes hemos tocado distintas partes del proyecto en mayor o menor medida, estas son las responsabilidades principales de cada uno:

**Víctor (vabad-ro)**

-Implementación de todas las operaciones de stack (sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr)

-Desarrollo del algoritmo adaptativo y su lógica de selección según el índice de desorden

-Implementación del algoritmo medio (ordenación por chunks, O(n√n))

**Luis (lvillarr)**

-Control y validación de argumentos (detección de errores, duplicados, desbordamiento de enteros)

-Implementación del algoritmo simple (extracción del mínimo/máximo, O(n²))

-Implementación del algoritmo complejo (Radix Sort LSD, O(n log n))

Ambos participamos en la revisión cruzada del código, la detección de bugs y las pruebas de rendimiento a lo largo del desarrollo.

---

## Pruebas de rendimiento esperadas

| Entrada | Mínimo (corte) | Buen rendimiento | Excelente |
|---------|---------------|------------------|-----------|
| 100 números | < 2000 ops | < 1500 ops | < 700 ops |
| 500 números | < 12000 ops | < 8000 ops | < 5500 ops |

---

## Recursos

### Referencias

- Knuth, D. E. — *The Art of Computer Programming, Vol. 3: Sorting and Searching*
- [Radix Sort — Wikipedia](https://en.wikipedia.org/wiki/Radix_sort)
- [Push_swap visualizer](https://github.com/o-reo/push_swap_visualizer) — herramienta para visualizar las operaciones
- [Repositorio de referencia de algoritmos de ordenación](https://visualgo.net/en/sorting)

### Uso de IA

Durante el desarrollo de este proyecto se utilizó IA (Claude) para las siguientes tareas:
- Apoyo en la comprensión de la notación Big-O aplicada al modelo de operaciones de Push_swap.
- Generación de casos de prueba para validar los algoritmos.

- Redacción y estructuración de este README.

En todos los casos, el código implementado fue escrito, revisado y comprendido por los integrantes del grupo antes de su entrega.

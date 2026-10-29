# AGENTS.md — Reglas del proyecto Codexion (42)

> Fuente: `es.subject.pdf` (v1.0). Estas restricciones son **obligatorias y permanentes**.
> Respétalas **SIEMPRE**, en cada cambio y en cada archivo. Si algo entra en conflicto con ellas, ganan ellas.

## 0. Modo de trabajo (aprendizaje)
- El usuario está **aprendiendo**: explica el *por qué*, guía con preguntas y deja que **él** escriba el código.
  Skills de apoyo: `teaching-style`, `socratic-tutor`, `socrates`.
- No entregues código de producción que el usuario no entienda. Antes de una solución, razona en voz alta.
- Estas reglas tienen prioridad sobre cualquier otra consideración de comodidad o rapidez.

## 1. Norma (Norm de 42)
- Lenguaje: **C**.
- El código **debe pasar la Norm** (`norminette`). Un solo error de norma en **cualquier** archivo
  (incluidos los extra) ⇒ **0**.
- Sin terminaciones inesperadas: segfault, bus error, double free, etc. (salvo comportamiento indefinido).
- **Sin fugas de memoria**: todo `malloc` debe tener su `free`.
- Entrega en `coders/`: `Makefile`, `*.c`, `*.h`. Nombre del programa: `codexion`.
- Norm: https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf

## 2. Compilación / Makefile
- Compilar con `cc -Wall -Wextra -Werror -pthread`.
- El Makefile **no debe relinkear**.
- Reglas mínimas: `$(NAME)`, `all`, `clean`, `fclean`, `re`.

## 3. Prohibiciones
- **Variables globales: prohibidas.**
- **libft: NO permitida.**
- Solo estas funciones están autorizadas:
  `pthread_create`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_lock`,
  `pthread_mutex_unlock`, `pthread_mutex_destroy`, `pthread_cond_init`, `pthread_cond_wait`,
  `pthread_cond_timedwait`, `pthread_cond_broadcast`, `pthread_cond_destroy`, `gettimeofday`,
  `usleep`, `write`, `malloc`, `free`, `printf`, `fprintf`, `strcmp`, `strlen`, `atoi`, `memset`.

## 4. Argumentos (todos obligatorios)
```
number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
- Validar y **rechazar** entradas inválidas: negativos, valores no enteros, y `scheduler`
  distinto de `fifo` o `edf`.

## 5. Modelo de la simulación
- 1 coder = 1 hilo (`pthread_create`). `n` coders ↔ `n` dongles (si n=1, un solo dongle).
- Ciclo de cada persona: `compile` (necesita 2 dongles, izquierdo + derecho) → `debug` → `refactor`
  → intenta compilar de nuevo.
- **Cooldown obligatorio**: tras liberar un dongle no puede volver a tomarse hasta pasar
  `dongle_cooldown` ms.
- **Arbitraje justo obligatorio** según `scheduler`:
  - `fifo`: se atiende la solicitud que llegó primero.
  - `edf`: deadline de burnout más próximo = `last_compile_start + time_to_burnout`.
- Con `edf`, **nadie debe agotarse** si los parámetros son viables.
- Debe existir un **hilo monitor separado** que detecte el burnout con precisión y detenga la simulación.
- La simulación termina cuando alguien se agota **o** cuando todos han compilado al menos
  `number_of_compiles_required` veces.
- Implementar una **cola de prioridad (heap) propia** para la planificación FIFO/EDF (C89 no trae una).

## 6. Logs (formato exacto, serializado)
```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```
- Nunca se deben mezclar dos mensajes en la misma línea (**mutex para la salida**).
- El mensaje de burnout debe aparecer **dentro de los 10 ms** posteriores al agotamiento real.
- Tiempo: usar `gettimeofday()` (tiempo real; aceptable y recomendado por simplicidad).

## 7. README.md (en la raíz del repositorio)
- Primera línea en cursiva: `*Este proyecto ha sido creado como parte del currículo de 42 por <login1>[, <login2>[...]].*`
- Secciones obligatorias: **Descripción**, **Instrucciones**, **Recursos**
  (incluyendo una descripción del uso de IA: para qué tareas y en qué partes).
- Secciones extra obligatorias:
  - **Blocking cases handled** — problemas de concurrencia resueltos (interbloqueos y condiciones de
    Coffman, prevención de inanición, gestión del cooldown, detección precisa del burnout,
    serialización del log).
  - **Thread synchronization mechanisms** — primitivas usadas (`pthread_mutex_t`, `pthread_cond_t`,
    eventos propios) y cómo coordinan el acceso a recursos compartidos (dongles, logging, estado del
    monitor), con ejemplos de cómo se evitan las condiciones de carrera.

## 8. Recordatorio final
- Antes de dar por bueno un cambio, comprueba: ¿pasa la Norm? ¿sin leaks? ¿compila con
  `-Wall -Wextra -Werror -pthread`? ¿usa solo funciones autorizadas? ¿respeta el formato de logs?
- Ante ambigüedad del subject, **pregunta** al usuario antes de asumir.

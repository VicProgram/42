# Requisitos — Codexion

> *Domina la carrera por los recursos antes de que la fecha límite te domine a ti.*

Resumen ejecutivo de **todo** lo que pide el subject (`es.subject.pdf`, v1.0), reorganizado y sin ruido.
Si algo no está aquí, míralo en el PDF original; si hay conflicto, **manda el PDF**.

---

## Índice

1. [Visión general](#1-visión-general)
2. [Identidad del programa](#2-identidad-del-programa)
3. [Argumentos](#3-argumentos)
4. [Modelo de la simulación](#4-modelo-de-la-simulación)
5. [Registro (logs)](#5-registros-logs)
6. [Reglas de la parte obligatoria](#6-reglas-de-la-parte-obligatoria)
7. [Restricciones técnicas](#7-restricciones-técnicas)
8. [Requisitos del README](#8-requisitos-del-readme)
9. [Checklist final](#9-checklist-final)

---

## 1. Visión general

- `n` personas programando se sientan en un **hub circular** de co-working.
- En el centro hay un **Compilador Cuántico** compartido.
- Sobre la mesa hay **tantos dongles USB como personas**.
- Compilar requiere **dos dongles a la vez**, uno en cada mano (izquierdo + derecho).
- Cada persona alterna: **compile → debug → refactor → intenta compilar otra vez**.
- Las personas **no se comunican** entre sí ni saben si otra está a punto de agotarse.
- Nadie debería agotarse… pero si alguien lo hace, la simulación termina.

---

## 2. Identidad del programa

| Concepto | Valor |
|---|---|
| Nombre del programa | `codexion` |
| Archivos a entregar | `Makefile`, `*.c`, `*.h` dentro de `coders/` |
| Reglas del Makefile | `$(NAME)`, `all`, `clean`, `fclean`, `re` |
| Compilación | `cc` con `-Wall -Wextra -Werror -pthread`, **sin relink** |
| Lenguaje | C (y debe pasar la Norm en **todos** los archivos, incluidos los extra) |
| libft | **No permitida** |

### Funciones autorizadas

Solo estas, ninguna más:

```
pthread_create          pthread_join
pthread_mutex_init      pthread_mutex_lock
pthread_mutex_unlock    pthread_mutex_destroy
pthread_cond_init       pthread_cond_wait
pthread_cond_timedwait  pthread_cond_broadcast
pthread_cond_destroy
gettimeofday            usleep
write                   malloc    free
printf                  fprintf
strcmp                  strlen    atoi    memset
```

---

## 3. Argumentos

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

**Todos obligatorios** (8 argumentos). Descripción:

| # | Argumento | Significado |
|---|---|---|
| 1 | `number_of_coders` | Número de personas **y** número de dongles |
| 2 | `time_to_burnout` | ms sin **empezar a compilar** desde el inicio de la última compilación (o desde el inicio de la simulación) → burnout. `deadline = last_compile_start + time_to_burnout` |
| 3 | `time_to_compile` | ms que tarda en compilar; durante ese tiempo sostiene **dos** dongles |
| 4 | `time_to_debug` | ms que se dedica a depurar |
| 5 | `time_to_refactor` | ms que se dedica a refactorizar. Al terminar, intenta **inmediatamente** adquirir dongles y compilar de nuevo |
| 6 | `number_of_compiles_required` | Si todas las personas han compilado al menos ese número de veces → fin de la simulación |
| 7 | `dongle_cooldown` | ms que un dongle queda **no disponible** tras ser liberado |
| 8 | `scheduler` | Política de arbitraje: exactamente `fifo` o `edf` |

### Validación (rechazar con mensaje de error y salida limpia)

- Números **negativos**.
- Valores **no enteros** (p. ej. `12a`, `1.5`, cadena vacía).
- `scheduler` distinto de `fifo` o `edf`.
- Número de argumentos incorrecto.

> **Por qué `atoi` no basta:** devuelve `0` para cualquier basura sin quejarse.
> Tienes que validar el formato tú mismo además de usarlo.

---

## 4. Modelo de la simulación

### Disposición circular

- Cada persona tiene un número de `1` a `number_of_coders`.
- La persona `1` se sienta junto a la `number_of_coders`.
- Cualquier persona `N` se sienta entre `N - 1` y `N + 1`.
- Hay un dongle **entre cada par de personas**: cada una tiene uno a su izquierda y otro a su derecha.
- **Caso especial:** si solo hay una persona, hay **un solo dongle** sobre la mesa.

### Ciclo de vida de cada persona

```
          ┌─────────────────────────────────────────┐
          │                                         │
          ▼                                         │
     (adquiere 2 dongles) → compile → debug → refactor
                            t_compile  t_debug  t_refactor
```

- Mientras compila no depura ni refactoriza (y viceversa: son estados excluyentes).
- Al terminar de refactorizar vuelve a intentar compilar **de inmediato**.

### Burnout (agotamiento)

- Una persona se agota si **no comienza a compilar** dentro de `time_to_burnout` ms desde:
  - el **inicio de su última compilación**, o
  - el **inicio de la simulación** (la primera vez, aún no ha compilado).
- Es decir: `deadline = last_compile_start + time_to_burnout`, y el deadline se **renueva** cada vez que empieza a compilar.
- Con `edf` y parámetros **viables**, **nadie debe agotarse**.

### Planificadores

| Scheduler | Regla |
|---|---|
| `fifo` | First In, First Out: se concede el dongle a la solicitud que llegó **primero** |
| `edf` | Earliest Deadline First: se concede a quien tenga el deadline de burnout **más próximo** (`last_compile_start + time_to_burnout`) |

---

## 5. Registros (logs)

### Formato exacto

Cualquier cambio de estado de una persona debe formatearse **exactamente** así:

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Donde `timestamp_in_ms` es la marca temporal actual en milisegundos y `X` el número de la persona.

### Reglas

- **Un mensaje nunca se mezcla con otro en la misma línea** (registro serializado con mutex).
- El mensaje de burnout debe mostrarse **dentro de los 10 ms** posteriores al momento real del agotamiento.
  (Se tolera una mínima desviación por hardware/planificación del SO.)
- Antes de `is compiling` aparecen **dos** líneas `has taken a dongle` (izquierdo y derecho).

### Ejemplo del subject

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
402 2 has taken a dongle
403 2 has taken a dongle
403 2 is compiling
603 2 is debugging
803 2 is refactoring
1204 3 burned out
```

> **Sobre el tiempo:** el subject recomienda usar **tiempo real con `gettimeofday()`**
> para este proyecto; es aceptado y recomendado por simplicidad.

---

## 6. Reglas de la parte obligatoria

- [ ] Cada persona es **un hilo** (`pthread_create`).
- [ ] El estado de cada dongle está protegido por un **mutex** (`pthread_mutex_t`) para que no se duplique.
      Se puede usar una **variable de condición** (`pthread_cond_t`) para las colas de espera.
- [ ] El **cooldown es obligatorio**: tras liberar un dongle, no puede volver a tomarse hasta que
      pasen `dongle_cooldown` ms. *(Se aplica al **soltar**, no al coger.)*
- [ ] El **arbitraje justo es obligatorio**: cuando varias personas soliciten el mismo dongle, se concede
      según el `scheduler` (`fifo` o `edf`).
- [ ] Con `edf` **nadie debe agotarse**, siempre que los parámetros sean viables.
- [ ] Debe existir un **hilo monitor separado** que detecte el burnout con precisión y detenga la simulación;
      el log debe imprimirse dentro de los 10 ms posteriores al agotamiento real.
- [ ] El **registro está serializado**: nunca dos mensajes en la misma línea (mutex de salida).
- [ ] La simulación termina cuando **alguien se agota** o cuando **todas las personas** han compilado
      al menos `number_of_compiles_required` veces.
- [ ] El código compila con `-Wall -Wextra -Werror -pthread`.
- [ ] Se implementa una **cola de prioridad (heap) propia** para la planificación FIFO/EDF
      (C89 no trae una biblioteca estándar para esto).
- [ ] **Toda la memoria** asignada se libera correctamente: sin fugas.

---

## 7. Restricciones técnicas

- **Variables globales: prohibidas.**
- Sin terminaciones inesperadas: segfault, bus error, double free, etc.
- Sin fugas de memoria (`valgrind --leak-check=full` limpio).
- La Norm se verifica en **todos** los archivos: un solo error ⇒ 0.
- El Makefile **no debe relinkear** y debe usar `cc`.

---

## 8. Requisitos del README

Debe estar en la raíz del repositorio y permitir que alguien ajeno entienda el proyecto rápido.

- [ ] **Primera línea en cursiva:**
      *Este proyecto ha sido creado como parte del currículo de 42 por \<login1\>[, \<login2\>…].*
- [ ] Sección **Descripción**: objetivo y visión general.
- [ ] Sección **Instrucciones**: compilación, instalación y/o ejecución.
- [ ] Sección **Recursos**: referencias clásicas (docs, artículos, tutoriales) **más** una descripción
      del uso de IA: para qué tareas y en qué partes del proyecto.
- [ ] Sección **“Blocking cases handled”**: todos los problemas de concurrencia gestionados
      (interbloqueos y condiciones de Coffman, inanición, cooldown, detección precisa del burnout,
      serialización del log…).
- [ ] Sección **“Thread synchronization mechanisms”**: primitivas usadas
      (`pthread_mutex_t`, `pthread_cond_t`, implementación personalizada de eventos) y cómo
      coordinan el acceso a los recursos compartidos (dongles, logging, estado del monitor),
      con ejemplos de cómo se evitan condiciones de carrera y se logra comunicación thread-safe
      entre las personas y el monitor.

> El idioma es a discreción personal; se recomienda inglés pero no es obligatorio.

---

## 9. Checklist final

Antes de dar nada por terminado:

- [ ] `make` compila con `cc -Wall -Wextra -Werror -pthread`, sin relink.
- [ ] `norminette coders/` → 0 errores en todos los archivos.
- [ ] Sin variables globales. Sin libft. Solo funciones autorizadas.
- [ ] Rechaza entradas inválidas (negativos, no enteros, scheduler raro).
- [ ] Cooldown respeta `dongle_cooldown` ms tras cada liberación.
- [ ] `fifo` respeta el orden de llegada; `edf` prioriza el deadline más próximo.
- [ ] Con `edf` y parámetros viables, nadie se agota.
- [ ] Monitor separado detecta burnout e imprime ≤ 10 ms después.
- [ ] Logs con el formato exacto y nunca mezclados entre líneas.
- [ ] Termina por burnout **o** por compilaciones completadas.
- [ ] `valgrind` sin fugas en todos los caminos (normal, burnout, error de argumentos).
- [ ] README con todas las secciones obligatorias.

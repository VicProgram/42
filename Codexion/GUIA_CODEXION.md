# GUÍA UNIVERSITARIA: Codexion

## Una clase paso a paso para entender y construir el proyecto

**Duración estimada:** 12–18 horas (repartidas en varios días; no lo hagas de golpe)

> **Cómo usar esta guía.** Esto es un **mapa**, no la solución. Léela, piensa, y **escribe tú el código**.
> En cada lección tienes: qué estudiar, qué tener en cuenta, preguntas para ti y un *checkpoint*
> (“no pases hasta saber hacer X”). Cada vez que dudes, pregúntame a mí o a un compañero.
> Recuerda las reglas del subject (`AGENTS.md`): Norm, sin globales, sin libft, funciones autorizadas,
> `-Wall -Wextra -Werror -pthread`, sin leaks.
> Este índice **no es definitivo**: lo iremos ajustando sobre la marcha.

---

## ⭐ DÓNDE ESTAMOS AHORA (actualizado 2026-10-27)

> **PROYECTO COMPLETADO Y VERIFICADO**

| # | Hito | Estado | Notas |
|---|---|---|---|
| — | Estructura de carpetas + Makefile + header | ✅ **Hecho** | Sin relinking, compila con `-Wall -Wextra -Werror -pthread` |
| M0 | Compila, norminette y sin relink | ✅ **Hecho** | `norminette` limpio en `coders/` |
| M1 | Parsing y validación de argumentos | ✅ **Hecho** | Rechaza negativos, no enteros, scheduler inválido, args incorrectos |
| M2 | Estado compartido e inicialización | ✅ **Hecho** | Sin globales, arrays malloc'd correctamente, cleanup ordenado |
| M3 | Hilos y ciclo del coder | ✅ **Hecho** | `compile → debug → refactor` cíclico |
| M4 | Adquirir/soltar dongles (mutex + cond) | ✅ **Hecho** | Adquisición ordenada por índice para evitar deadlock |
| M5 | Cooldown | ✅ **Hecho** | `dongle_cooldown` respetado con `cond_timedwait` |
| M6 | Heap y planificación FIFO | ✅ **Hecho** | Heap binario propio por dongle, orden estable por `seq` |
| M7 | Planificación EDF | ✅ **Hecho** | `key = last_compile + time_to_burnout`, desempate por `seq` |
| M8 | Monitor y detección de burnout | ✅ **Hecho** | Monitor separado, detención con precisión < 10ms, parada limpia |
| M9 | Log serializado | ✅ **Hecho** | Todo via `log_mutex`, formato exacto, timestamps relativos a `start_time` |
| M10 | Terminación y limpieza (sin leaks) | ✅ **Hecho** | `join` de todos los hilos, `cleanup` completo, sin leaks |
| V | Verificación (norma, leaks, tests, README) | ✅ **Hecho** | `norminette` OK, `valgrind` sin leaks, tests exhaustivos OK |
| D | Defensa (recode + preguntas) | ✅ **Listo** | Todo funcional, documentado y entendido |

**En una frase:** El programa valida argumentos, reserva estado, lanza `n` hilos coder + 1 monitor, gestiona dongles con mutex/cond/heap (FIFO/EDF), aplica cooldown, detecta burnout con precisión y serializa logs. Compila limpio, pasa norminette, sin leaks ni cuelgues.

## EMPIEZA AQUÍ (Guía rápida para alguien nuevo)

Si nunca has visto este proyecto, lee esto primero:

1. **Lee el subject**: `es.subject.pdf` - Lo que pide el enunciado
2. **Entiende las reglas**: `AGENTS.md` - Norm, restricciones obligatorias
3. **Prueba que funcione**:
```bash
cd coders
make
./codexion 4 1000 200 100 100 3 50 fifo
```
4. **Sigue las lecciones**: Empieza por la Parte I (Lecciones 1-6) para entender concurrencia, luego ve a la Parte IV para ver cómo se aplica
5. **Debugging**: Usa `valgrind --leak-check=full` y `norminette coders/` tras cada cambio

**Estructura del código:**
- `main.c` - Entrada, `run_sim`, joins
- `parser.c` - Validación de argumentos
- `init.c` - Inicialización/cleanup
- `coder.c` - Hilo coder, ciclo, adquisición de dongles
- `monitor.c` - Detección de burnout
- `heap.c` - Cola de prioridad propia (FIFO/EDF)
- `tools.c` - Logs, helpers
- `time.c` - Tiempos
- `codexion.h` - Estructuras y prototipos



### ✅ Cerrado en M7 (EDF)

- `sim->start_time = get_time_ms()` en `init()` **después** de `init_dongles` y **antes** de
  `init_coders`; `init_coders` siembra `last_compile = sim->start_time`.
- `coder_loop` actualiza `c->last_compile = get_time_ms()` **entre** `acquire_dongles` y el `usleep`
  de compilación (el reloj de burnout cuenta desde que *empieza* a compilar).
- `take_dongle` elige la clave:

```c
	if (strcmp(coder->sim->scheduler, "edf") == 0)
		req.key = coder->last_compile + coder->sim->time_to_burnout;
	else
		req.key = req.seq;
```

- **`heap.c` no se tocó**: `key` y `seq` ya estaban separados justo para esto.
- **Verificado con traza** (copia en `/tmp`, sin ensuciar el repo): las claves de `edf` son epoch-ms
  (~1,79 × 10¹²) y crecen cuando el coder recompila. `fifo` sigue con 0, 1, 2…
- **Verificado sin traza**: `fifo` y `edf` × `n = 1..8` (10 vueltas cada uno) → 0 fallos; `n = 12`
  `edf` cooldown 7 → 0 fallos; `user 0.00 s`; cooldown 0 → 0.14 s vs 200 → 1.14 s.
- **valgrind**: `0 bytes lost`, `0 errors`.

> ⚠️ **Trampa documentada:** en la primera compilación **todos** los coders tienen el mismo
> `last_compile` (arrancan a la vez), así que todos empatan en `key` y EDF desempata por `seq`. No es
> un bug: es el reparto inicial. La diferenciación aparece a partir de la segunda vuelta.

> ⚠️ **Error real de este hito:** un `req.key = req.seq;` duplicado después del `if/else` **pisaba
> siempre** el deadline → EDF desactivado en silencio. Lo cazó `-Wmisleading-indentation` al quedar
> indentado como parte del `else`. Si un `if/else` sin llaves necesita dos sentencias, van con `{ }`.


### 🛑 Punto exacto donde lo dejamos (M8, 26/10 20:10)

**Hecho y compilando:**

| Pieza | Estado |
|---|---|
| `codexion.h` | ✅ `void *monitor_loop(void *arg);` — el `*` que faltaba era el error de compilación |
| `monitor.c` | ✅ 5 funciones: `all_done`, `call_stop`, `check_burnout`, `sim_stopped`, `monitor_loop` |
| `run_sim` | ✅ lanza el monitor **después** de los coders, solo si `i == n_coders`, y lo une **después** de `join_threads` |
| `coder_loop` | ❌ todavía no consulta `sim_stopped` |
| Pruebas de burnout | ⏳ pendientes |

**Lo que ya funciona:** `make` verde y una ejecución normal termina con `Program Finished`.

#### Pendiente 1 — arreglar `mon_start` (una línea)

Ahora mismo `mon_start` guarda el **retorno crudo** de `pthread_create` (0 = bien) y luego se usa como
booleano (`if (mon_start)`). Está **invertido**:

```diff
 	if (i == sim->n_coders)
-		mon_start = pthread_create(&sim->thread, NULL, monitor_loop, sim);
+		mon_start = (pthread_create(&sim->thread, NULL, monitor_loop, sim) == 0);
 	join_threads(sim, i);
 	if (mon_start)
 		pthread_join(sim->thread, NULL);
```

| Caso | Con el retorno crudo | Con `== 0` |
|---|---|---|
| el monitor se crea bien | `0` → **no** hace join | `1` → hace join ✅ |
| el monitor falla | `≠ 0` → join de un hilo que no existe | `0` → no hace join ✅ |
| no se intentó crear | `0` (init) → no hace join | `0` → no hace join ✅ |

Sin el arreglo, en el caso bueno `cleanup` puede destruir `stop_mutex` / `log_mutex` mientras el
monitor aún los usa. **Nunca mezcles un código de error con un nombre de sí/no.** La explicación
larga está en la Lección 20, sección *“`pthread_create` devuelve 0 = bien”*.

#### Pendiente 2 — `coder_loop`

```text
PSEUDO — coder_loop, versión M8
    mientras NO sim_stopped(c.sim)  Y  compile_times < required:
        acquire_dongles(c)
        si sim_stopped(c.sim):
            release_dongles(c)          ← soltar SIEMPRE antes de salir
            terminar el hilo            ← sin compilar ni contar
        last_compile ← ahora
        compilar ; contar ; soltar
        debug ; refactor
```

- La condición nueva va **primero** en el `&&`.
- El `if` va **después** de `acquire_dongles`, no antes (si no, el coder se queda dormido dentro de
  `take_dongle` y no se entera).
- **No** comprobar `stop` dentro de `take_dongle`: salir sin `heap_pop` deja una petición huérfana
  como raíz y bloquea el dongle (eso es M10).
- Sin el `release_dongles` antes del `return`, la cadena de `broadcast` no arranca y el `join` cuelga.

#### Pendiente 3 — pruebas (en este orden)

1. **Burnout FIFO:** `timeout 5 ./codexion 3 50 200 50 50 100 0 fifo`
   → **una sola** línea `... burned out`, termina en ~50 ms (no en los 100×200 ms del ciclo) y sale
   con código 0.
2. **Burnout EDF:** el mismo comando con `edf`.
3. **Sin burnout:** `timeout 5 ./codexion 3 1000 10 10 10 2 0 fifo` → ninguna línea de burnout y
   `Program Finished`.
4. **No regresión:** estrés de M7 (`n = 1..8` × 10, ambos schedulers) con `timeout`.
5. **valgrind** sobre una ejecución normal y una con burnout.

> ⚠️ **Mientras `coder_loop` no consulte `stop`:** el monitor **sí** imprime el burnout y **sí** sale,
> pero los coders siguen compilando hasta completar su cupo. La prueba 1 tardará muchísimo y no
> servirá para validar M8. El orden es: arreglar `mon_start` → tocar `coder_loop` → probar.

### ✅ Cerrado en M6 (heap + FIFO)

- `t_request { coder, key, seq }` — el heap guarda **peticiones**, no coders.
- `t_dongle` gana `heap` (`malloc` de `n_coders`), `heap_size`, `heap_cap`, `arrival`.
- `heap.c` con **5 funciones**: `request_less`, `sift_up`, `sift_down`, `heap_push`, `heap_pop`.
  - `request_less` compara `key` y desempata por `seq`.
  - `push` escribe en **`size`** (primera libre) y sube; `pop` **decrementa primero** y lee `size`.
- `take_dongle`: `push` → `while` con **tres** motivos (no soy root · ocupado · cooldown) → `pop`.
- **Verificado aislado**: 12 claves desordenadas salen ordenadas y el empate por `seq` también
  (`HEAP OK`).
- **Verificado en simulación**: `n = 1..8` × 10 y `n = 12` (`edf`, cooldown 7) × 10 → **0 cuelgues**;
  `user 0.00 s` (sin busy-wait); cooldown 0 → 0.14 s vs 200 → 1.14 s.
- **valgrind**: `0 bytes lost`, `0 errors`.

> **Doble `cleanup` resuelto:** las dos líneas de `run_sim` ya no están; `main` es el único dueño
> de `cleanup`. Lo que queda aplazado es solo que `run_sim` devuelva `int` para informar de un fallo
> de `pthread_create` (M10).

### ✅ Cerrado en M5 (cooldown)

- `t_dongle` tiene `cooldown_ms` (copiado de `sim->dongle_cooldown` en `init_dongles`) y
  `cooldown_until` (instante absoluto en ms).
- `release_dongle`: `cooldown_until = get_time_ms() + cooldown_ms` **antes** de marcar `is_free = 1`.
- `take_dongle`: `while (is_free == 0 || get_time_ms() < cooldown_until)` — si está ocupado espera
  en la `cond`; si está libre pero en cooldown, espera con `pthread_cond_timedwait`.
- `time.c`: `get_time_ms()` (epoch en ms, con `gettimeofday`) + `wait_until_ms()`.
- **Verificado**: mismo escenario con `cooldown = 0` tarda 0.14 s y con `cooldown = 200` tarda
  1.14 s. Si no estuviera implementado, no cambiaría.

> **Detalle fino que acertaste:** `pthread_cond_timedwait` pide una hora **absoluta** (desde epoch),
> no relativa. Como `cooldown_until` es epoch-ms y se convierte con `ms_to_timespec`, coincide con
> `CLOCK_REALTIME` (que es lo que usa `gettimeofday`). Es un error clásico y no lo cometiste.

### ✅ Cerrado en M4 (dongles)

- **`take_dongle(t_dongle *d)`** → `lock` → `while (!is_free) cond_wait` → `is_free = 0` → `unlock`.
- **`release_dongle(t_dongle *d)`** → `lock` → `is_free = 1` → `broadcast` → `unlock`.
- `pthread_cond_t cond` añadido a `t_dongle`, `cond_init` en `init_dongles`, `cond_destroy` en `cleanup`.

### 🛠️ La corrección importante de M4: el ORDEN de adquisición

Esto costó varias iteraciones y es el corazón del proyecto:

```c
static void	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->right;
	second = coder->left;
	if (first > second)
	{
		first = coder->left;
		second = coder->right;
	}
	take_dongle(first);
	if (first != second)
		take_dongle(second);
}
```

**Qué hace:** `first` es siempre el dongle de **índice menor** (no "el izquierdo"). Los dos punteros
salen del mismo array (`sim->dongles`), así que compararlos con `>` compara índices.

**Por qué:** con "izquierdo primero", el coder `n-1` coge `d(n-1)` y luego `d0`, cerrando un ciclo
de espera (Coffman) → **deadlock**. Con orden creciente, el hilo que tiene el índice más alto nunca
espera nada, así que siempre puede terminar y desbloquear al resto.

**Evidencia real (antes y después):**

| n | Antes (izquierda primero) | Después (índice creciente) |
|---|---|---|
| 1 | ✅ | ✅ |
| 2 | 🔴 cuelga | ✅ 0/20 |
| 3 | 🔴 cuelga | ✅ 0/20 |
| 4 | 🔴 cuelga | ✅ 0/20 |
| 5 | 🔴 cuelga | ✅ 0/20 |
| 8 | — | ✅ 0/20 |
| 12 (`edf`) | — | ✅ 0/15 |

> ⚠️ **Lección aprendida:** una prueba que "pasa una vez" no prueba nada. El deadlock dependía del
> interlineado; con los `printf` de debug salía siempre y sin ellos a veces no. **Repetir el test
> en bucle es lo que da la señal.**

### 🪤 El bug que el compilador NO caza

`release_dongle(arg)` donde `arg` es un `void *` que apunta a un `t_coder`:

- C convierte `void *` → `t_dongle *` **sin avisar**.
- Dentro se hace `pthread_mutex_lock` sobre los primeros bytes del struct del coder → basura.
- **Compila limpio con `-Wall -Wextra -Werror`**. Compilar sin warnings ≠ funcionar.

Regla: **dentro de `coder_loop` solo se llaman las que acaban en `_dongles`** (reciben `t_coder *`).
Las `_dongle` son para dentro de los helpers.

### ⚠️ Deuda latente que sigue pendiente

| Deuda | Dónde | Cuándo |
|---|---|---|
| `run_sim` no informa de si falló: si `pthread_create` falla, vuelve en silencio y `main` sale con `0` (el `cleanup` ya es único y correcto) | `main.c` `run_sim` | al hacer `run_sim` estricto (`int`), en M10 |
| Los timestamps del log: falta decidir si son **epoch** o **relativos a `start_time`** | `coder.c` / futuro `log.c` | **M9** |

### ⚠️ Deudas anotadas (no bloquean, pero no se olvidan)

| Deuda | Dónde | Cuándo se ataca |
|---|---|---|
| Overflow en el parseo (`99999999999` cuela) | `parser.c` `check_range` | al final, si sobra tiempo (`safe_atoi`) |
| `printf` sin formato del subject en `coder.c` | `coder.c` | **M9** (log serializado) |
| `while (compile_times < required)` en el coder | `coder.c` | **M8** (lo decide el monitor, no el coder) |
| `init` llama a `cleanup` con mutexes no inicializados si falla un `malloc` | `init.c` | cuando se toque `init` de nuevo |
| No se comprueba el retorno de `pthread_create` en producción | `main.c` | ya hecho en el camino de error ✅ |
| `mon_start` guarda el retorno crudo de `pthread_create` (0 = bien) y se comprueba como booleano → lógica invertida | `main.c` `run_sim` | **M8, arreglo de una línea** |
| `coder_loop` aún no consulta `stop`: un burnout no pararía a los coders | `coder.c` | **M8, siguiente paso** |
| Al parar por burnout quedan **peticiones huérfanas** en los heaps; el vaciado cuesta hasta `n × cooldown` (se acepta en M8) | `coder.c` / `heap.c` | **M10** (abandonar la petición al ver `stop`) |

### 📐 Cómo leer el pseudocódigo

Cada mecanismo y cada función del proyecto tiene su bloque de pseudocódigo, además del código real
cuando ya existe. Reglas para leerlo:

- **No es C.** No hay `;`, ni llaves, ni tipos, ni `#include`. Se lee como una receta.
- `→` significa "entonces" o "pasa a".
- `mientras` / `si` / `para cada` abren un bloque; el bloque termina cuando **desciende** la
  sangría (no hay `fin si` ni llaves).
- `devolver X` es el `return`.
- Los nombres son los reales del proyecto (`last_compile`, `cooldown_until`, `heap_size`…).
- Describe **cómo está el código hoy**, no una versión ideal.

### 🗺️ Mapa del pseudocódigo

Todos los bloques `PSEUDO —` de la guía, por lección. Si quieres repasar un mecanismo, ve directo:

| Lección | Qué encontrarás |
|---|---|
| L1 | ciclo de vida de los hilos (crear → join → cleanup), pilas privadas vs memoria compartida |
| L2 | la carrera paso a paso, la misma suma con mutex, el dongle sin mutex |
| L3 | esperar/avisar, por qué `while` y no `if`, traducción a `dongle.mutex`/`cond` |
| L4 | el deadlock clásico, la solución de orden global, tabla de Coffman |
| L5 | inanición sin planificador y reparto por reglas con el heap |
| L6 | `get_time_ms`, `wait_until_ms`, por qué no fiarse de `usleep` |
| L6.5 | reservar/rellenar un array, las tres zonas de `cleanup` |
| L7 | la vida de un coder, la mesa circular y el caso `n = 1` |
| L8 | `main + parse_args`, formato vs valor |
| L9 | escribir una línea de log, por qué un solo `printf` |
| L10 | `release_dongle` / `take_dongle` con cooldown, línea temporal |
| L11 | deadline, renovación al empezar a compilar, monitor |
| L12 | FIFO vs EDF con el mismo ejemplo de tres peticiones |
| L13 | el heap en un array, `push`/`pop`, ejemplo de `sift up` |
| L14 | quién reserva/libera y quién toca cada campo compartido |
| L15 | `monitor_loop`, lo que el monitor NO hace, coder vs monitor |
| L16 | diseño A (elegido) vs diseño B (descartado) |
| L18 | pipeline de `parse_args` |
| L19 | `init_dongles`, `init_coders`, `init`, `cleanup` |
| L20 | `coder_loop`, `run_sim`, `join_threads` |
| L21 | `take_dongle`, `release_dongle`, `acquire_dongles`, `release_dongles` |
| L22 | cooldown en el tiempo |
| L23 | `request_less`, `sift_up`, `sift_down`, `heap_push`, `heap_pop` |
| L24 | elección de clave y tabla de deadlines |
| L25 | `all_done`, `check_burnout`, `call_stop`, `sim_stopped`, `monitor_loop` |

### Regla para avanzar

1. Abre la lección del hito que toca (abajo está el enlace).
2. Léela entera **antes** de escribir código.
3. Escribe el código tú; si algo no cabe en la Norm, es señal de que la función hace demasiado → pártela.
4. Antes de marcar ✅: `make` limpio + `norminette coders/` a 0 + la prueba del checkpoint.
5. Commit pequeño y marca la casilla en la tabla de arriba.

### Índice → qué lección mirar según el hito

| Hito | Lección |
|---|---|
| M0 | [Lección 17](#lección-17-m0--estructura-makefile-y-norm) |
| M1 | [Lección 18](#lección-18-m1--parsing-y-validación) |
| M2 | [Lección 19](#lección-19-m2--estado-compartido-e-inicialización) |
| M3 | [Lección 20](#lección-20-m3--hilos-y-el-ciclo-del-coder) |
| M4 | [Lección 21](#lección-21-m4--adquirirsoltar-dongles-mutex--condition-variable) |
| M5 | [Lección 22](#lección-22-m5--cooldown) |
| M6 | [Lección 23](#lección-23-m6--el-heap-y-la-planificación-fifo) |
| M7 | [Lección 24](#lección-24-m7--planificación-edf) |
| M8 | [Lección 25](#lección-25-m8--el-monitor-y-la-detección-de-burnout) ← **estás aquí** |
| M9 | [Lección 26](#lección-26-m9--log-serializado) |
| M10 | [Lección 27](#lección-27-m10--terminación-y-limpieza-sin-leaks) |

---

## Índice

**PARTE 0 — Antes de escribir código**
- Lección 0: Mentalidad y herramientas

**PARTE I — Los fundamentos (concurrencia)**
- Lección 1: ¿Qué es un hilo? Proceso vs. hilo
- Lección 2: Condiciones de carrera y mutex
- Lección 3: Variables de condición (esperar sin quemar CPU)
- Lección 4: Interbloqueo (deadlock) y condiciones de Coffman
- Lección 5: Inanición (starvation) y equidad
- Lección 6: El tiempo: `gettimeofday`, `usleep` y precisión
- Lección 6.5: Arrays, memoria dinámica y el bucle que recorre *(añadida tras M2)*

**PARTE II — Entender el problema Codexion**
- Lección 7: El modelo (coders, estados, dongles)
- Lección 8: Los 8 argumentos y su validación
- Lección 9: El log y su formato (serialización)
- Lección 10: El cooldown del dongle
- Lección 11: Burnout y deadlines
- Lección 12: Los planificadores: FIFO y EDF
- Lección 13: La cola de prioridad (heap) propia

**PARTE III — Diseño de la arquitectura**
- Lección 14: Estructuras de datos y organización de archivos
- Lección 15: El monitor como observador
- Lección 16: Diseñar dos veces (módulos profundos)

**PARTE IV — Manos a la obra (por hitos)** · *(✅ = hecho, 🔴 = en curso, ⬜ = pendiente)*
- Lección 17: M0 — Estructura, Makefile y Norm · **✅**
- Lección 18: M1 — Parsing y validación · **✅**
- Lección 19: M2 — Estado compartido e inicialización · **✅**
- Lección 20: M3 — Hilos y el ciclo del coder · **✅**
- Lección 21: M4 — Adquirir/soltar dongles (mutex + cond) · **✅**
- Lección 22: M5 — Cooldown · **✅**
- Lección 23: M6 — El heap y la planificación FIFO · **✅**
- Lección 24: M7 — Planificación EDF · **✅**
- Lección 25: M8 — El monitor y la detección de burnout · 🔴 **SIGUIENTE**
- Lección 26: M9 — Log serializado · ⬜
- Lección 27: M10 — Terminación y limpieza (sin leaks) · ⬜

**PARTE V — Verificación**
- Lección 28: Norminette y estilo
- Lección 29: Leaks y sanitizers
- Lección 30: Pruebas y casos límite
- Lección 31: El README

**PARTE VI — Defensa**
- Lección 32: La recode
- Lección 33: Preguntas típicas de evaluación

**ANEXO**
- Glosario
- Checklist final
- Mapa del subject → dónde se cumple
- Recursos para estudiar

---

# PARTE 0: ANTES DE ESCRIBIR CÓDIGO

## LECCIÓN 0: Mentalidad y herramientas

### 0.1. Una frase que resume el proyecto

Todo Codexion cabe en una idea: **muchos hilos compitiendo por recursos escasos, y tú eres el árbitro**.
Los “dongles” son el recurso escaso; el “monitor” es el árbitro; el “cooldown” y el “planificador” son las reglas del juego.

La dificultad no está en escribir mucho código, sino en escribir **poco código que sea correcto a la vez**. La concurrencia castiga la improvisación: un fallo aparece 1 de cada 10.000 ejecuciones.

### 0.2. Herramientas que vas a usar (instálalas y pruébalas ya)

| Herramienta | Para qué | Cómo se usa |
|---|---|---|
| `cc` (gcc/clang) | compilar | `cc -Wall -Wextra -Werror -pthread *.c` |
| `norminette` | comprobar la Norm | `norminette coders/` |
| `valgrind` | fugas de memoria | `valgrind --leak-check=full ./codexion ...` |
| `valgrind --tool=helgrind` | condiciones de carrera | `valgrind --tool=helgrind ./codexion ...` |
| `-fsanitize=thread` | detectar *data races* | compilar con extra y ejecutar |
| `git` | guardar versiones | commit pequeño tras cada hito |

Si faltan, se instalan así:

```bash
uv tool install norminette        # norminette, sin romper el python del sistema
sudo apt-get install valgrind     # valgrind (pide contraseña)
```

> **Aviso real (pasó en M2):** las extensiones "42 formatter" de VS Code reformatean tu código al
> guardar con reglas **distintas** al norminette real (quitaban los espacios de `a * b`, que la Norm
> exige). Desinstálalas o desactiva `formatOnSave`. El juez es la CLI: `norminette coders/`.

**El bucle de trabajo que funciona** (probado en M2): escribes tu versión → la pegas y razonas las
dudas → compuebas con `make` y `norminette` **tras cada cambio pequeño**, no al final. El compilador
y el norminette son revisores gratis que no se cansan.

### 0.3. Cómo pedir ayuda sin hacer trampa

El subject es claro: puedes usar IA para tareas repetitivas, pero **solo puedes entregar lo que entiendes**.
Regla personal: si no puedes explicar una línea, no la entregas. Usa esta guía para estudiar y a mí para
preguntarme *por qué*, no para que te escriba el programa.

### Checkpoint

- [ ] Sé compilar un “hola mundo” con `-pthread` y ejecutarlo.
- [ ] Sé qué hace `norminette` y `valgrind`.
- [ ] Tengo claro que **no puedo** usar variables globales ni libft.

---

# PARTE I: LOS FUNDAMENTOS

> Antes de tocar Codexion, entiende cinco conceptos. Sin ellos, el código es magia.

## LECCIÓN 1: ¿Qué es un hilo? Proceso vs. hilo

### 1.1. Analogía

- Un **proceso** es un edificio: tiene su propia dirección, sus propias tuberías, su propio almacén.
- Un **hilo** es una persona dentro del edificio. Varias personas comparten el almacén y las tuberías
  (la **memoria**), pero cada una tiene su propio escritorio (su pila, *stack*) y sus notas privadas.

Consecuencia clave: dos hilos que usan **la misma variable** se pisan. Dos hilos con **variables locales**
van a lo suyo.

### 1.2. En C

- `pthread_create(&t, NULL, función, argumento)` → nace un hilo que ejecuta `función(argumento)`.
- `pthread_join(t, NULL)` → el hilo principal espera a que `t` termine.
- El **argumento** es un `void *`: normalmente le pasas un puntero a una estructura con todo lo que el hilo necesita.

La función de un hilo tiene **firma fija**: `void *mi_hilo(void *arg)`. Dentro casteas el argumento
(`t_coder *c = arg;`) y devuelves `NULL` al terminar.

```c
pthread_t   t;
t_coder     c;

pthread_create(&t, NULL, coder_loop, &c);  // coder_loop corre EN PARALELO con main
pthread_join(t, NULL);                      // main se bloquea hasta que coder_loop acabe
```

Dos detalles: `pthread_create` devuelve 0 si fue bien (hay que comprobarlo), y **no se puede hacer
`join` de un hilo que no se creó**.

**Pseudocódigo del ciclo de vida de los hilos:**

```text
PSEUDO — main(), de principio a fin
    crear la simulación y sus dongles
    para cada coder i:
        crear_hilo(coder_loop, &coders[i])      → si falla, dejar de crear
    crear_hilo(monitor_loop, sim)               (M8)
    unir los coders que sí nacieron             (join, uno por uno)
    unir el monitor                             (join)
    cleanup(sim)                                (solo cuando ya nadie usa la memoria)
```

Y quién comparte qué:

```text
proceso "codexion"
├── hilo main        pila privada ─┐
├── hilo coder 1     pila privada ─┤
├── hilo coder 2     pila privada ─┼── comparten: t_sim, dongles, heaps, mutex
└── hilo monitor     pila privada ─┘
```

Las **pilas** son privadas (cada hilo tiene sus locales); la **memoria reservada** con `malloc`
es de todos. Por eso `cleanup` va **después** del último `join`: liberar lo que otro hilo aún toca
es un uso después de liberar.


### 1.3. Join, detach... y por qué el proceso muere con los hilos dentro

Un hilo no tiene vida propia: vive **dentro** del proceso. Cuando `main` hace `return 0`, el SO mata
al instante todos los hilos, aunque estén a medio `printf`. Ejecuta este experimento:

```c
void	*lenta(void *a)
{
	(void)a;
	sleep(2);
	printf("hilo: termine\n");
	return (NULL);
}

int	main(void)
{
	pthread_t	t;

	pthread_create(&t, NULL, lenta, NULL);
	return (0);   // sin join: "hilo: termine" NUNCA aparece
}
```

Hay dos formas de gestionar el final de un hilo, y se eligen con una sola pregunta:
**¿necesito esperar a que acabe?**

| | `pthread_join(t, NULL)` | `pthread_detach(t)` |
|---|---|---|
| ¿Main espera? | Sí, se bloquea | No |
| ¿Sabes cuándo terminó? | Sí | No |
| ¿Puedes hacer join luego? | Sí | No (es error) |
| Recursos del hilo | Los recoge el join | Los recoge el SO |
| Útil para | "nadie apaga las luces hasta que salga el último" | tareas sueltas sin orden |

Codexion usa **join**: `main` debe esperar a que todos los hilos terminen antes de `cleanup`
(destruir mutex con hilos vivos que los usan = desastre). Patrón para todo el proyecto:
**crear todos → join de todos → cleanup**.

En Codexion: **cada coder es un hilo**, y hay **un hilo extra: el monitor**.

### Qué tener en cuenta

- Sin `pthread_join`, el `main` puede terminar y matar los hilos antes de que hagan nada.
- `pthread_create` devuelve un código de error: hay que comprobarlo.
- Como no hay variables globales, cada hilo recibe un puntero a una **estructura compartida** (“la mesa”).

### Preguntas para ti

1. ¿Por qué dos hilos que incrementan la misma variable a la vez dan un resultado “imposible”?
2. ¿Qué se comparte entre hilos y qué no?

### Checkpoint

- [ ] Sé explicar con mis palabras la diferencia entre pila (privada) y memoria compartida.

---

## LECCIÓN 2: Condiciones de carrera y mutex

### 2.1. La carrera

Si dos hilos hacen `contador = contador + 1` a la vez, la operación no es atómica:
leer → sumar → escribir. Los interleaveados dan pérdidas. Eso es una **condición de carrera** (*race condition*).

### 2.2. Mutex

Un **mutex** (`pthread_mutex_t`) es un candado: solo un hilo puede tenerlo a la vez.

- `pthread_mutex_lock(&m)` → cierra con llave (si está ocupado, espera).
- `pthread_mutex_unlock(&m)` → abre.
- `pthread_mutex_init` / `pthread_mutex_destroy` → crear/destruir.

Regla de oro: **sección crítica pequeña**. Coge el mutex, haz el mínimo, suelta. No hagas `usleep` con el mutex cogido.

### 2.3. La carrera en vivo (experimento que puedes ejecutar)

5 hilos sumando 200.000 veces cada uno a la misma variable, **sin mutex**:

```c
void	*trabajo(void *a)
{
	int	i;

	(void)a;
	i = 0;
	while (i < 200000)
	{
		compartida = compartida + 1;
		i++;
	}
	return (NULL);
}
```

Salida real: `esperado: 1000000 | real: 317101`. Se pierden ~700.000 sumas. ¿Por qué?
`compartida + 1` son 3 pasos (leer → sumar → escribir) y el SO intercambia entre hilos a mitad:
A lee 1000, B lee 1000, A escribe 1001, B escribe 1001 → una suma desaparece.

El mismo programa con `lock` → `compartida++` → `unlock` da **1.000.000 siempre**.

En Codexion, esto es exactamente lo que pasaría si dos coders "leen" el mismo dongle libre a la vez:
ambos creerían que se lo llevan. El mutex es lo que impide la doble lectura.

**Pseudocódigo de la carrera y del candado:**

```text
PSEUDO — carrera con dos hilos (contador empieza en 0)
    A: lee     contador → 0
    B: lee     contador → 0
    A: suma    0 + 1
    B: suma    0 + 1
    A: escribe contador ← 1
    B: escribe contador ← 1        (la suma de A desapareció)
    resultado: 1        esperado: 2

PSEUDO — la misma suma con mutex
    bloquear m
    leer contador ; sumar 1 ; escribir contador      (los tres pasos sin soltar m)
    desbloquear m

PSEUDO — el dongle SIN mutex (lo que no hay que hacer)
    si dongle.is_free == 1:
        dongle.is_free ← 0
        "me lo llevo"
    (dos hilos pueden leer 1 antes de que ninguno escriba 0:
     los dos creen tenerlo y el mismo dongle se usa dos veces)
```

La sección crítica es **la decisión completa** (`leer → decidir → escribir`), no solo la escritura.


### Qué tener en cuenta

- El **estado de cada dongle** (libre / ocupado / en cooldown) es un dato compartido → necesita mutex.
- El **log** también es un recurso compartido: sin mutex, dos mensajes se mezclan en la misma línea.
- Olvidar un `unlock` en un camino de error = todo se congela.

### Checkpoint

- [ ] Sé señalar qué datos de Codexion necesitan mutex y por qué.

---

## LECCIÓN 3: Variables de condición (esperar sin quemar CPU)

### 3.1. El problema

Un coder quiere un dongle que está ocupado. Opciones malas:

- **Busy-wait**: preguntar en bucle “¿ya está?” → quema CPU y es feo.
- **Dormir a ciegas**: `usleep` fijo → o te pasas o te quedas corto.

La solución es la **variable de condición** (`pthread_cond_t`): te duermes hasta que alguien te avise.

### 3.2. El patrón correcto (memorízalo)

```c
pthread_mutex_lock(&m);
while (condición_no_se_cumple)          // while, NO if
    pthread_cond_wait(&cv, &m);         // suelta m y duerme; al despertar recupera m
/* ... sección crítica ... */
pthread_mutex_unlock(&m);
```

Y el que cambia el estado hace:
```c
pthread_mutex_lock(&m);
estado = listo;
pthread_cond_broadcast(&cv);            // o signal
pthread_mutex_unlock(&m);
```

**Pseudocódigo del patrón esperar/avisar:**

```text
PSEUDO — el que espera (un coder)
    bloquear m
    mientras la condición no se cumpla:
        dormir en la cond        (suelta m; al despertar lo vuelve a coger)
    usar el recurso
    desbloquear m

PSEUDO — el que avisa (el que suelta)
    bloquear m
    cambiar el estado a "disponible"
    despertar a todos los que esperan en la cond
    desbloquear m

PSEUDO — por qué while y no if
    despierto → comprobar la condición otra vez
    si sigue sin cumplirse → volver a dormir
    (nadie garantiza que el estado siga igual que cuando te avisaron)

PSEUDO — traducción a Codexion
    m         → dongle.mutex
    cond      → dongle.cond
    condición → soy la raíz del heap  Y  is_free == 1  Y  ya pasó el cooldown
```

### 3.3. La variante que necesitarás

`pthread_cond_timedwait(&cv, &m, &deadline)` despierta solo: **como máximo** hasta el tiempo indicado.
Te hará falta para no dormir indefinidamente cuando hay un *deadline* que vigilar.

### Qué tener en cuenta

- `while`, nunca `if`: pueden despertarte por “falsos despertares”.
- `pthread_cond_timedwait` usa un **tiempo absoluto** (desde la época Unix), no “5 ms desde ahora”.

### Checkpoint

- [ ] Sé escribir el patrón productor/consumidor con `while` + `cond_wait`.

---

## LECCIÓN 4: Interbloqueo (deadlock) y condiciones de Coffman

### 4.1. El problema clásico

Dos coders necesitan **dos** dongles. Si el coder 1 coge el izquierdo y el coder 2 coge el que le queda
a la derecha de 1… y cada uno espera al otro, **nadie avanza jamás**. Eso es *deadlock*.

### 4.2. Las cuatro condiciones de Coffman (las 4 se deben dar a la vez)

1. **Exclusión mutua**: el recurso no se puede compartir.
2. **Retención y espera**: tienes uno y esperas otro.
3. **Sin expropiación**: nadie te quita el dongle.
4. **Espera circular**: A espera a B, B espera a A.

Para evitar el deadlock, **rompe al menos una**. En Codexion se rompe la **circularidad**: todos los
coders piden sus dos dongles **en el mismo orden global** (el de índice menor primero). No hay árbitro
central: cada dongle sigue teniendo su propio mutex, su cond y su heap.

### Qué tener en cuenta

- El README pide una sección **“Blocking cases handled”**: aquí es donde justificas cómo evitas Coffman.
- Un *deadlock* no da error: simplemente el programa **se queda colgado**. Aprende a “leer” un cuelgue.

**Pseudocódigo del deadlock y de la solución:**

```text
PSEUDO — el deadlock clásico
    coder 1: coge el dongle A          (lo tiene)
    coder 2: coge el dongle B          (lo tiene)
    coder 1: espera el dongle B        (lo tiene 2)
    coder 2: espera el dongle A        (lo tiene 1)
    → nadie suelta y nadie avanza: espera circular

PSEUDO — la solución de Codexion (orden global)
    first  ← el dongle de índice MENOR de los dos que necesito
    second ← el otro
    coger first
    coger second
    → todos piden en el mismo orden: la espera circular es imposible
```

| Condición de Coffman | ¿Se rompe en Codexion? | Cómo |
|---|---|---|
| Exclusión mutua | No | un dongle es de un solo coder a la vez |
| Retención y espera | No | coges el primero y esperas el segundo |
| Sin expropiación | No | nadie te quita un dongle |
| **Espera circular** | **Sí** | orden global por índice (y `first == second` con `n = 1`) |


### Preguntas para ti

1. Si todos cogieran primero el dongle izquierdo, ¿qué condición de Coffman se cumple y por qué colgaría?

### Checkpoint

- [ ] Sé explicar mi estrategia anti-deadlock y qué condición de Coffman rompe.

---

## LECCIÓN 5: Inanición (starvation) y equidad

### 5.1. Justicia

Si el dongle siempre se lo lleva “el más rápido”, un coder lento **nunca compila** → su deadline vence → burnout.
Eso es **inanición**.

La solución es el **planificador**: cuando varios piden a la vez, la mesa decide **por reglas**, no por suerte.

- **FIFO**: el que pidió primero, primero. Sencillo y justo “por turno”.
- **EDF**: el que tiene el deadline de burnout más cercano, primero. Justo “por urgencia”.

**Pseudocódigo de la inanición y del planificador:**

```text
PSEUDO — sin planificador (lo que provoca inanición)
    mientras el dongle esté libre:
        el primero que lo vea se lo lleva      (suerte, no reglas)
    → un coder lento puede perder siempre

PSEUDO — con planificador
    cada petición se apunta en el heap del dongle con su clave
    el dongle solo se entrega a la RAÍZ del heap
    → la decisión ya no es "quién miró antes" sino "quién manda por regla"

    fifo: clave = orden de llegada       → turnos
    edf : clave = deadline de burnout    → urgencia
```

El planificador no reparte dongles: **ordena la cola de cada dongle**. Quien decide es el heap;
el `scheduler` solo elige con qué número se compara.


### Qué tener en cuenta

- Sin una **cola de espera** por dongle, no puedes decidir “quién va primero”: cada petición necesita
  guardar *a quién* pertenece y *cuándo* llegó (o *qué deadline* tiene).
- Con `edf` el subject exige que **nadie se agote** si los parámetros son viables.

### Checkpoint

- [ ] Sé decir qué información debe guardar cada solicitud para poder ordenarla.

---

## LECCIÓN 6: El tiempo: `gettimeofday`, `usleep` y precisión

### 6.1. Medir y esperar

- `gettimeofday` → **mira la hora** actual en milisegundos. Sirve para: el `timestamp_in_ms` del log,
  calcular deadlines y comprobar cooldowns.
- `usleep` → **duerme** el hilo un número de microsegundos (¡ojo: micro, no mili!).

### 6.2. El problema de la precisión

`usleep(1000)` **no** duerme exactamente 1 ms: el sistema operativo puede tardar más. Por eso:

- El burnout debe loguearse **dentro de los 10 ms** posteriores al momento real.
- Por eso existe un **monitor separado**: vigila continuamente los deadlines y actúa en cuanto se cumplen,
  en vez de confiar en que un `usleep` sea exacto.

**Pseudocódigo del reloj:**

```text
PSEUDO — get_time_ms()
    gettimeofday(&tv)
    devolver tv.sec * 1000 + tv.usec / 1000        (epoch en ms)

PSEUDO — wait_until_ms(cond, mutex, until)
    convertir until (ms epoch) a un struct timespec
    pthread_cond_timedwait(cond, mutex, &ts)

PSEUDO — por qué no fiarse de usleep
    deadline ← ahora + 50
    dormir 50 ms
    → al despertar pueden ser 51, 55 o 70 ms: el aviso llegaría tarde
    → por eso el monitor muestrea cada 1 ms y compara contra el reloj
      en vez de dormir "hasta el deadline"
```

Regla del proyecto: los argumentos del usuario vienen en **ms**, `usleep` pide **µs** → siempre
`ms * 1000`. Y todas las comparaciones de tiempo se hacen con **epoch en ms** (`get_time_ms`),
nunca mezclando `time()`, `clock()` y `gettimeofday`.


### Qué tener en cuenta

- Un solo reloj coherente: usa `gettimeofday` en TODAS las medidas (no mezcles fuentes).
- Convierte `struct timeval` (seg, µs) a milisegundos: `tv_sec*1000 + tv_usec/1000`.
- `usleep` con valor 0 es válido; `usleep` enorme = hilo muerto.

### Checkpoint

- [ ] Sé convertir un `struct timeval` a milisegundos y comparar dos tiempos.

---

## LECCIÓN 6.5: Arrays, memoria dinámica y el bucle que recorre

> **Añadida tras M2**, porque todos los bugs reales de ese hito vinieron de aquí. Léela despacio:
> son 20 minutos que te ahorran horas.

### 6.5.1. ¿Por qué un array y no variables sueltas?

El número de coders lo decide el usuario al arrancar: `./codexion 7 ...` o `./codexion 50 ...`.
En C, las variables se declaran con tamaño fijo **al compilar**: no puedes escribir
`t_coder c1, c2, ..., cN` porque N no la sabes. La única respuesta es **reservar memoria en
tiempo de ejecución**:

```c
sim->dongles = malloc(sizeof(t_dongle) * sim->n_coders);
```

Eso crea **un bloque contiguo** de n posiciones y devuelve su dirección:

```
sim->dongles ──▶ [ dongle 0 ][ dongle 1 ][ dongle 2 ]
                   índice 0    índice 1    índice 2
```

Un bloque = **una sola reserva = un solo `free`** al final. Los "elementos" no existen aparte:
son posiciones dentro del bloque, y se acceden con `sim->dongles[i]`.

### 6.5.2. La regla de oro: reservar una vez, rellenar n veces

El error más repetido de M2 fue meter el `malloc` **dentro** del bucle:

```c
while (i < n)
{
	sim->dongles = malloc(...);   // ❌ CREA UN BLOQUE NUEVO en cada vuelta
	...
	i++;
}
```

Traza con `n = 3`: se crean 3 arrays; los 2 primeros quedan **huérfanos** (leak); el puntero solo
guarda el último. Y solo rellenas un elemento por vuelta, del bloque que acabas de tirar.

```c
sim->dongles = malloc(...);       // ✅ UNA reserva, fuera del bucle
while (i < n)
{
	/* rellena dongles[i] */      // ✅ el bucle SOLO rellena
	i++;
}
```

**Mentalidad:** el `malloc` **crea** el bloque entero de golpe; el bucle **recorre** poniendo cada
posición a su valor. Crear n veces = perder n-1 bloques.

**Pseudocódigo de reservar y rellenar:**

```text
PSEUDO — un array de n elementos (ejemplo n = 3)
    dongles ← reservar 3 × tamaño de t_dongle      (UNA sola vez)
    i ← 0
    mientras i < 3:
        inicializar dongles[i]
        i ← i + 1
    ...
    free(dongles)                                  (UNA sola vez, al final)
```

La forma del código es siempre la misma: **reserva fuera, bucle de relleno dentro, free fuera**.
Si el `malloc` aparece dentro del bucle, la primera vuelta pierde el bloque de la anterior.


### 6.5.3. Índices de 0 a n-1 (y el truco del `%`)

Los índices de un array van de `0` a `n - 1`. Un bucle con `i = 1` deja el elemento **0 con
basura** — y con `n = 1`, `while (i < 1)` **no se ejecuta ni una vez** (caso especial obligatorio
del subject: una persona, un dongle, y quedaría sin inicializar).

Para dar la vuelta a la mesa circular se usa el **resto de la división** (`%`). Con `n = 4`:

| i | i+1 | (i+1) % 4 |
|---|-----|-----------|
| 0 | 1 | 1 |
| 1 | 2 | 2 |
| 2 | 3 | 3 |
| 3 | 4 | **0** ← el último se empareja con el primero |

Con `n = 1`: `(0 + 1) % 1 = 0` → `left == right == &dongles[0]`. Correcto según el subject.

> ⚠️ **Pero ojo (lo verás en M4):** si `left == right`, hacer `take_dongle(left)` **y después**
> `take_dongle(right)` es tomar **el mismo dongle dos veces** → el segundo `take_dongle` se queda
> esperando al primero → **cuelgue eterno**. Con `n = 1` se coge **una sola vez**.

### 6.5.4. Índice ≠ id

Dos números que se confunden fácilmente:

- `i` = posición en memoria, **0-based** (`0 .. n-1`).
- `coder_id` = el número que imprime el log, **1-based** (`1 .. n`). Por eso: `coder_id = i + 1`.

Si empiezas el bucle en `i = 1` "para que el id empiece en 1", pierdes el elemento 0 y los ids
salen desplazados (`2, 3, 4, 5`). El `+1` va **dentro de la asignación**, no en el índice.

### 6.5.5. malloc vs calloc (da igual aquí)

`malloc` deja basura; `calloc` deja ceros. Pero en Codexion **todos** los campos se inicializan a
mano (`is_free = 1` no cabe en un cero; el mutex necesita `pthread_mutex_init`; `coder_id` es
`i + 1`), así que el ahorro del `calloc` son tres líneas y **oculta** la inicialización: si mañana
añades un campo y te olvidas de él, con `calloc` "funciona" hasta que no. Elige `malloc` + init
explícito.

### 6.5.6. Cuatro trampas de sintaxis que causaron bugs reales

1. **`if (!init_dongles)` sin paréntesis** no ejecuta nada: el nombre de una función en C es su
   dirección de memoria (siempre distinta de NULL) → el `if` es siempre falso. Necesitas
   `if (!init_dongles(sim))`. Regla: paréntesis = ejecutar; sin paréntesis = solo el puntero.
2. **`x == NULL` pregunta, `x = NULL` asigna.** `&x == NULL` no pone nada a NULL, solo compara y
   tira la respuesta.
3. **`sim->dongles[i].mutex;` es una sentencia sin efecto**: lee el mutex y descarta el valor.
   Si querías crearlo, falta `pthread_mutex_init(...)`. Si una línea no llama a una función ni
   asigna, normalmente no hace nada.
4. **Falta `return (1)` en un helper**: el llamante lee basura y actúa en base a ella. Toda ruta
   de éxito debe devolver su valor.

### 6.5.7. El orden de `cleanup` (post-mortem de M2)

`cleanup` es el espejo de `init` y se divide en tres zonas con **orden fijo**:

```
1. bucle:   destroy de cada dongles[i].mutex    (usa el array)
2. únicos:  destroy de stop_mutex y log_mutex
3. memoria: free(dongles); free(coders);   y SOLO ENTONCES   = NULL
```

Trampas que ya fallaron una vez:

- `free` **dentro** del bucle → destruyes mutex sobre memoria liberada y haces double free.
- `= NULL` **dentro** del bucle → la vuelta siguiente desreferencia NULL (segfault) y los `free`
  posteriores son `free(NULL)` (no libran nada → leak). El `NULL` es el **cierre de puertas**:
  al final, después de todo el trabajo.

**Pseudocódigo de `cleanup`, en tres zonas:**

```text
PSEUDO — cleanup(sim)
    zona 1: para cada dongle i:
                destruir mutex y cond de dongles[i]
                liberar el heap de dongles[i]
    zona 2: destruir stop_mutex y log_mutex
    zona 3: free(dongles) ; free(coders)
            dongles ← NULL ; coders ← NULL      (al final, como cierre de puertas)
```

El orden no es estético: la zona 1 **usa** el array de dongles, así que no puede ir después del
`free`; y los `NULL` van al final para que ninguna vuelta del bucle desreferencie NULL.

- `free` elemento a elemento: **no**. Un array = **un solo `free`**.

### Checkpoint

- [ ] Sé dibujar el bloque de memoria de `dongles`/`coders` y las flechas `left`/`right`.
- [ ] Sé decir por qué el `malloc` va fuera del bucle y el `free` también.
- [ ] Con `n = 1`, sé predecir qué hace cada bucle de tu `init` **sin ejecutarlo**.

---

# PARTE II: ENTENDER EL PROBLEMA CODEXION

## LECCIÓN 7: El modelo (coders, estados, dongles)

### 7.1. La historia

Hay `n` personas en un hub circular. En el centro, un compilador cuántico. Sobre la mesa, **`n` dongles USB**
(hay tantos dongles como personas). Para **compilar** hacen falta **dos dongles**: el de tu izquierda y el de tu derecha.

### 7.2. El ciclo de vida de una persona

```
        ┌──────────────────────────────────────────┐
        │                                          │
   (intenta compilar) → compile → debug → refactor ┘
        necesita 2 dongles        t_debug    t_refactor
```

- Mientras compila, no depura ni refactoriza (y viceversa).
- Al terminar de refactorizar, **vuelve a intentar compilar inmediatamente**.

**Pseudocódigo del modelo:**

```text
PSEUDO — la vida de un coder
    repetir:
        necesito 2 dongles
        coger left ; coger right
        estado "compilando"  ; dormir comp_time
        soltar left ; soltar right
        estado "depurando"   ; dormir debug_time
        estado "refactorizando" ; dormir refactor_time
    hasta que compile_times == compiles_required  o  la simulación pare

PSEUDO — la mesa circular (ejemplo con n = 4)
    coder 1   left = dongle 1   right = dongle 2
    coder 2   left = dongle 2   right = dongle 3
    coder 3   left = dongle 3   right = dongle 4
    coder 4   left = dongle 4   right = dongle 1

    n = 1 → left == right == dongle 1 (se coge UNA sola vez)
```

Cada coder tiene **dos punteros** a dongles, pero no siempre a dongles distintos: con `n = 1` apuntan
al mismo. Todo el código de coger/soltar tiene que tolerar ese caso.


### 7.3. La disposición circular

- Personas numeradas `1..n`. La persona `1` se sienta junto a la `n`.
- La persona `N` está entre `N-1` y `N+1`. Cada una tiene un dongle a cada lado → `n` dongles.
- Caso especial: si `n = 1`, hay **un solo** dongle sobre la mesa.

### Qué tener en cuenta

- “Izquierdo” y “derecho” deben ser deterministas: define una convención (p. ej. coder `i` coge `i` y `(i+1) % n`)
  y no la cambies. Aquí nacen la mayoría de los deadlocks.
- El estado de cada persona cambia y **cada cambio se loguea**.

### Checkpoint

- [ ] Sé dibujar en papel la mesa con 4 personas, sus 4 dongles y quién toca qué dongle.

---

## LECCIÓN 8: Los 8 argumentos y su validación

### 8.1. La lista (todas obligatorias)

```
number_of_coders  time_to_burnout  time_to_compile  time_to_debug
time_to_refactor  number_of_compiles_required  dongle_cooldown  scheduler
```

| Argumento | Significado |
|---|---|
| `number_of_coders` | nº de personas **y** nº de dongles |
| `time_to_burnout` | ms sin empezar a compilar antes de agotarse |
| `time_to_compile` | ms compilando (sujetando 2 dongles) |
| `time_to_debug` | ms depurando |
| `time_to_refactor` | ms refactorizando |
| `number_of_compiles_required` | si todos compilan ≥ este nº, fin |
| `dongle_cooldown` | ms que un dongle queda inutilizable tras soltarlo |
| `scheduler` | `fifo` o `edf` (exactamente) |

### 8.2. Qué hay que rechazar

- Números **negativos**.
- Valores **no enteros** (p. ej. `12a`, `1.5`, vacío).
- `scheduler` distinto de `fifo` / `edf`.
- Faltan argumentos (distinto de 9 en total, contando el argv[0] del programa).

**Pseudocódigo de la validación:**

```text
PSEUDO — main + parse_args
    si argc != 9                       → error → devolver 1
    si check_number(args) == 0         → "invalid argument"       → devolver 1
    si check_scheduler(args) == 0      → "invalid argument"       → devolver 1
    fill_sim(sim, args)                 (atoi de los 7 números)
    si check_range(sim) == 0           → "invalid argument value" → devolver 1
    init(sim) → run_sim(sim) → cleanup(sim)

PSEUDO — qué mira cada validación
    check_number    → FORMATO: solo dígitos y al menos uno
                      fuera: "12a", "1.5", "", "-4"
    check_scheduler → exactamente "fifo" o "edf"
    check_range     → VALOR: n_coders > 0 y compiles_required > 0
```

Formato y valor son dos preguntas distintas: `"12a"` no es un número, y `"0"` sí lo es pero no es
un `number_of_coders` válido. Por eso hay tres funciones y no una.


### Qué tener en cuenta

- Debes **usar** `atoi` y `strcmp` (están autorizadas) — pero `atoi` no detecta basura: revísalo.
- Un `number_of_coders` de 0 probablemente sea inválido: ¿por qué?
- Rechazar = mensaje de error y salida limpia (código ≠ 0), **sin** fugas.

### Checkpoint

- [ ] Sé listar 5 entradas que debo rechazar y cómo lo detecto.

---

## LECCIÓN 9: El log y su formato (serialización)

### 9.1. El formato exacto (no te lo inventes)

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Ejemplo:
```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
```

### 9.2. Serialización

“Serializado” = **cada mensaje se escribe entero, sin que otro se cuele en medio**. Con varios hilos
escribiendo, sin protección obtendrías basura tipo `1 1 is comp0 2 has t...`. Solución: un **mutex de log**.

**Pseudocódigo del log:**

```text
PSEUDO — escribir una línea (así quedará en M9)
    ahora ← get_time_ms()
    bloquear log_mutex
    printf("%ld %d %s\n", ahora, coder_id, mensaje)
    desbloquear log_mutex

PSEUDO — por qué UNA sola llamada
    bien:  printf("%ld %d %s\n", ...)      → la línea sale de una pieza
    mal:   printf("%ld ", t)
           printf("%d ", id)
           printf("%s\n", msg)             → otro hilo se cuela entre las tres

PSEUDO — el invariante
    ninguna línea se mezcla  ⟺  toda escritura va dentro del MISMO mutex
    el burnout también se escribe ahí (lo escribe el monitor)
```

En M8 el único `printf` es el de burnout, y ya va bajo `log_mutex`. En M9 se centraliza en una
función `log_state(sim, coder_id, mensaje)` para que todos los mensajes pasen por el mismo sitio.


### Qué tener en cuenta

- Un `printf` de una sola llamada con formato completo es más fácil de serializar que varios `printf` sueltos.
- El log de **burnout** debe salir ≤ 10 ms después del agotamiento real.
- Ojo con el orden: primero “has taken a dongle” (dos veces), luego “is compiling”.

### Checkpoint

- [ ] Sé explicar por qué dos `printf` seguidos pueden mezclarse entre hilos.

---

## LECCIÓN 10: El cooldown del dongle

### 10.1. Qué es

Tras soltar un dongle, no puede volver a cogerse hasta que pasen `dongle_cooldown` **milisegundos**.

### 10.2. Cómo modelarlo

Cada dongle guarda “hasta qué instante está en cooldown” (`cooldown_until`). Quien quiera cogerlo
comprueba si `ahora >= cooldown_until`. Si no, **espera** (usando `pthread_cond_timedwait` para despertar
justo cuando expire).

**Pseudocódigo del cooldown:**

```text
PSEUDO — release_dongle (aquí arranca el cooldown)
    bloquear el mutex
    cooldown_until ← ahora + cooldown_ms      (ANTES de is_free)
    is_free ← 1
    despertar a todos
    desbloquear

PSEUDO — take_dongle (aquí se comprueba)
    mientras no sea mi turno  O  is_free == 0  O  ahora < cooldown_until:
        si solo falta el cooldown → dormir con temporizador hasta cooldown_until
        si no                      → dormir sin límite en la cond
    coger el dongle

ejemplo con cooldown = 200 y se suelta en el instante 1000:
    1000 ─────────────── 1200 ── 1201 ──▶ tiempo
    suelta               cogible
    is_free = 1 desde 1000, pero hasta 1200 no se puede coger
```

`is_free` y “se puede coger” **no** son lo mismo: por eso la condición del `while` tiene los dos
términos y no solo `is_free`.


### Qué tener en cuenta

- El cooldown se aplica **al soltar**, no al coger.
- Si `dongle_cooldown` es 0, el dongle está disponible de inmediato.
- El monitor también puede vigilar si el cooldown hace imposible cumplir un deadline.

### Checkpoint

- [ ] Sé decidir si un dongle está disponible en un instante dado, con una sola comparación.

---

## LECCIÓN 11: Burnout y deadlines

### 11.1. La regla

Una persona se agota si **no empieza a compilar** dentro de `time_to_burnout` ms desde:
- el **inicio de la simulación**, o
- el **inicio de su última compilación**.

Dicho de otro modo: `deadline = last_compile_start + time_to_burnout`.

**Pseudocódigo del deadline:**

```text
PSEUDO — deadline de un coder
    si nunca ha compilado:
        deadline ← start_time + time_to_burnout
    si no:
        deadline ← last_compile + time_to_burnout

PSEUDO — renovarlo (en coder_loop)
    acquire_dongles
    last_compile ← ahora            (al EMPEZAR a compilar, no al terminar)
    dormir comp_time

PSEUDO — el monitor
    ahora ← get_time_ms()
    para cada coder con trabajo pendiente:
        si ahora >= deadline(coder):
            log "coder burned out"
            parar la simulación

ejemplo con time_to_burnout = 100:
    start_time = 0            → deadline 100
    compila en 80             → deadline 180
    si en 180 aún no ha empezado otra compilación → burned out
```


### 11.2. Quién lo detecta

El **hilo monitor**, que vigila continuamente: si `ahora > deadline` de alguien → “X burned out” y **parar la simulación**.

### Qué tener en cuenta

- La primera vez, `last_compile_start` no existe: el deadline es `inicio_simulación + time_to_burnout`.
- El deadline se **renueva** cada vez que empieza a compilar.
- Precisión: el mensaje debe salir ≤ 10 ms del momento real. De ahí el monitor dedicado.

### Checkpoint

- [ ] Sé calcular el deadline de un coder en cualquier instante de su vida.

---

## LECCIÓN 12: Los planificadores: FIFO y EDF

### 12.1. La decisión

Cuando **varias** personas piden **el mismo** dongle y queda libre, ¿quién lo coge?

- **fifo** → el que pidió **primero**.
- **edf** → el que tiene el **deadline de burnout más próximo** (`last_compile_start + time_to_burnout`).

### 12.2. Cómo se implementa la decisión

Cada dongle tiene una **cola de solicitudes**. Cuando queda libre, eliges una según la política:

- FIFO: cola por **orden de llegada**.
- EDF: cola por **deadline** (la más urgente primero).

**Pseudocódigo de las dos políticas:**

```text
PSEUDO — FIFO
    clave ← número de llegada al dongle (0, 1, 2, ...)
    gana la clave MENOR

PSEUDO — EDF
    clave ← last_compile + time_to_burnout
    gana la clave MENOR  (= el que se agota antes)

ejemplo: tres peticiones al mismo dongle
    coder A   llegada 0   deadline 500
    coder B   llegada 1   deadline 200
    coder C   llegada 2   deadline 800

    fifo → A, B, C
    edf  → B, A, C
```

Fíjate en que **la cola es la misma**: solo cambia qué número se guarda como `key`. Ese es el
motivo de que `t_request` tenga un campo `key` y no dos políticas separadas.


### Qué tener en cuenta

- La política es **global** (viene en `argv`), pero cada dongle decide **entre sus propios solicitantes**.
- EDF exige que **nadie muera** con parámetros viables: es la política “segura”.
- Piénsalo como un **árbitro local**: el heap de ese dongle. Nadie se lo lleva sin ser su raíz.

### Checkpoint

- [ ] Con 3 peticiones simultáneas (deadlines 500, 200, 800), sé decir el orden en FIFO y en EDF.

---

## LECCIÓN 13: La cola de prioridad (heap) propia

### 13.1. Por qué una “cola de prioridad”

El subject pide explícitamente implementar un **heap** (montículo) para la planificación, porque C89 no
trae una cola de prioridad estándar.

- Un **min-heap** guarda elementos con una **clave** (en FIFO: el nº de llegada; en EDF: el deadline).
- Da siempre el **mínimo** en `O(1)` y lo inserta/extrae en `O(log n)`.

### 13.2. La idea (sin código)

Un heap es un **árbol binario completo** guardado en un array. Regla: el padre ≤ sus hijos.
Insertar y extraer “suben/bajan” el elemento hasta recolocarlo (`sift up` / `sift down`).

**Pseudocódigo del heap en un array:**

```text
PSEUDO — posiciones
    padre de i      = (i - 1) / 2
    hijo izquierdo  = 2 * i + 1
    hijo derecho    = 2 * i + 2
    regla           = heap[padre] <= heap[hijo]

PSEUDO — insertar (push)
    poner el nuevo al final del array
    subirlo mientras sea menor que su padre          (sift up)
    tamaño ← tamaño + 1

PSEUDO — extraer el mínimo (pop)
    guardar heap[0]
    mover el último a heap[0]
    bajarlo mientras algún hijo sea menor            (sift down)
    tamaño ← tamaño - 1
    devolver el guardado

ejemplo (claves): [3, 8, 5]  insertar 1
    [3, 8, 5, 1] → sube junto al 8 → [3, 1, 5, 8] → sube junto al 3 → [1, 3, 5, 8]
```

Complejidad: `push` y `pop` cuestan `O(log n)` porque el array es un árbol equilibrado por
construcción (nunca hay huecos); mirar el mínimo es `O(1)`, es `heap[0]`.


### Qué tener en cuenta

- Necesitas poder **comparar** con dos criterios (llegada o deadline). Un truco limpio: un campo `priority`
  que rellenas según el scheduler.
- También suele hacer falta **quitar un elemento concreto** (si un coder deja de querer el dongle).
- Ojo con los índices y con los tamaños al hacer `malloc`/`realloc`.

### Checkpoint

- [ ] Sé explicar en papel cómo insertar y extraer del heap, y qué orden da.

---

# PARTE III: DISEÑO DE LA ARQUITECTURA

## LECCIÓN 14: Estructuras de datos y organización de archivos

### 14.1. La gran decisión: nada de globales

Sin variables globales, **todo el estado** vive en una estructura y se pasa por puntero. Esquema típico
(de nombres, tú eliges):

- `t_sim`: la simulación entera (argumentos, estado, flags de parada, el log-mutex).
- `t_coder`: nº, hilo, `last_compile_start`, nº de compilaciones, punteros a sus dos dongles.
- `t_dongle`: mutex propio, estado (libre/cooldown), `cooldown_until`, cola de espera (heap).
- `t_request`: quién pide un dongle + su clave (llegada o deadline).



**Pseudocódigo de la propiedad de la memoria:**

```text
PSEUDO — quién reserva y quién libera
    main         → t_sim en su pila              (no hay free de sim)
    init_dongles → dongles[] y el heap de cada dongle → cleanup los libera
    init_coders  → coders[]                      → cleanup lo libera
    los hilos    → NO reservan y NO liberan nada → solo leen y escriben campos

PSEUDO — quién toca cada campo compartido
    sim.thread           lo escribe run_sim (create) y lo espera run_sim (join)
    sim.stop             lo escribe call_stop ; lo lee sim_stopped   (stop_mutex)
    sim.log_mutex        lo usan todos los que imprimen
    dongle.is_free       protegido por dongle.mutex
    dongle.heap          protegido por dongle.mutex
    dongle.arrival       protegido por dongle.mutex
    coder.last_compile   lo escribe su coder ; lo lee el monitor      (opción A)
    coder.compile_times  lo escribe su coder ; lo leen monitor y all_done
```

La regla de oro: **un solo dueño para crear/destruir**, muchos lectores bajo mutex. Si dos sitios
reservan lo mismo, tarde o temprano uno libera lo del otro.
### 14.2. Archivos sugeridos (dentro de `coders/`)

No hay una única forma. Una organización razonable:

```
coders/
├── Makefile
├── codexion.h        (o varios .h)
├── main.c            arranque y join
├── parse.c           validación de argumentos
├── init.c            reserva e inicialización
├── dongle.c          coger/soltar, cooldown, cola
├── heap.c            cola de prioridad
├── coder.c           ciclo de cada persona
├── monitor.c         vigilancia de deadlines
├── log.c             escritura serializada
├── time.c            helpers de tiempo
└── cleanup.c         liberar todo
```

### Qué tener en cuenta

- La Norm limita (nombre de funciones, una función por responsabilidad, etc.): consulta el PDF y pasa `norminette` a menudo, no al final.
- Un solo sitio que crea/destruye cada recurso: si lo creas en `init.c`, destrúyelo en `cleanup.c`.

> **Esto es un esquema sugerido, no tu árbol actual.** Hoy el repo es más pequeño:
>
> | Existe ya | Falta escribir |
> |---|---|
> | `main.c`, `parser.c` (aquí aparece como `parse.c`), `init.c`, `coder.c`, `sim.c`, `log.c` | `dongle.c`, `heap.c`, `monitor.c`, `time.c`, `cleanup.c` |
>
> Además, `cleanup()` vive de momento en `init.c:70`, no en un `cleanup.c` propio, y
> `take_dongle()` / `release_dongle()` están en `coder.c`. No hace falta crear los
> ficheros vacíos por adelantado: los abres cuando toque (M4, M6, M7).

### Checkpoint

- [ ] Sé dibujar mis estructuras y qué campos lleva cada una, y quién los modifica.

---

## LECCIÓN 15: El monitor como observador

### 15.1. Qué es

Un **hilo más** cuyo único trabajo es **observar** deadlines y decidir cuándo se agota alguien.
No compila, no debuggea: vigila.

### 15.2. Por qué separado

- Detecta el burnout con precisión (≤ 10 ms).
- Puede revisar **a la vez** el estado de todos los coders.

### 15.3. Cómo se coordina

El monitor lee el estado compartido bajo los mutex correspondientes. Detectado un burnout: marca “parar”
y el resto de hilos lo ven (con su mutex / cond) y salen.

**Pseudocódigo del observador:**

```text
PSEUDO — monitor_loop
    repetir para siempre:
        si todos cumplieron su cupo → parar la simulación y salir
        ahora ← get_time_ms()
        para cada coder con trabajo pendiente:
            si ahora >= last_compile + time_to_burnout:
                log "coder burned out"
                parar la simulación
                salir
        dormir 1 ms

PSEUDO — lo que el monitor NO hace
    no coge dongles
    no compila
    no elige quién pasa (eso lo decide el heap de cada dongle)
    solo MIRA y, si toca, PARA
```

| | Coder | Monitor |
|---|---|---|
| ¿Trabaja con dongles? | Sí | No |
| ¿Duerme esperando? | En el mutex/cond del dongle | 1 ms fijo |
| ¿Escribe en el log? | Sus estados | Solo el burnout |
| ¿Decide el orden? | No | No (decide el heap) |


### Qué tener en cuenta

- Debe leer `last_compile_start` de forma segura (mutex), no a lo loco.
- Debe ser **rápido**: no puede dormir 100 ms; se despierta con frecuencia o con `cond_timedwait`.

### Checkpoint

- [ ] Sé explicar en qué se diferencia el monitor de los hilos coder.

---

## LECCIÓN 16: Diseñar dos veces (módulos profundos)

Antes de implementar la parte difícil (el arbitraje de dongles), **piensa dos diseños** y compáralos:

- **Diseño A**: cada coder intenta coger sus dos dongles por su cuenta (con mutex por dongle).
- **Diseño B**: un **árbitro central** (o la mesa) concede los pares de dongles según el planificador.

Compáralos en: simplicidad, riesgo de deadlock, facilidad para cumplir FIFO/EDF, testabilidad.

**Pseudocódigo de los dos diseños:**

```text
PSEUDO — diseño A (el elegido)
    cada dongle es un módulo profundo:
        interfaz: take_dongle(coder, dongle) / release_dongle(dongle)
        esconde:  mutex, cond, heap, cooldown, llegada
    un coder pide primero el dongle de índice menor y después el otro
    el deadlock se rompe con ese orden global

PSEUDO — diseño B (descartado)
    un árbitro central recibe "quiero mis dos dongles" y los concede juntos
    ventaja: no hay retención y espera
    coste:   un único punto de serialización y más estado que proteger
```

El proyecto se quedó con **A**: menos estado central, cada dongle decide entre sus propios
solicitantes, y la corrección se apoya en una regla simple (orden global) en vez de en un componente
nuevo que hay que probar aparte.


Pista de teoría de diseño (skill `codebase-design`): un **módulo profundo** es mucho comportamiento detrás
de una interfaz pequeña. ¿Qué interfaz mínima necesitaría tu “mesa”? Algo como
`pedir_dongles(coder)` / `soltar_dongles(coder)` esconde todo el arbitraje.

### Checkpoint

- [ ] He escrito en papel los pros y contras de A y B y he elegido uno **con un motivo**.

---

# PARTE IV: MANOS A LA OBRA (POR HITOS)

> Regla de cada hito: **compila, pasa norminette y no tiene leaks** antes de pasar al siguiente.
> Commitea al terminar cada uno.

## LECCIÓN 17: M0 — Estructura, Makefile y Norm

**Estado: ✅ HECHO.**

Crea `coders/`, tu `.h`, y el `Makefile` con las reglas `$(NAME) all clean fclean re`.
Flags: `-Wall -Wextra -Werror -pthread`, compilador `cc`, **sin relink**.

Qué se consiguió:
- `coders/Makefile`, `coders/codexion.h`, `coders/main.c`.
- `make`, `make re` y `make fclean` funcionan; el enlace usa `$(CFLAGS)` (incluye `-pthread`).
- Norminette limpio en los archivos que existen.

- [x] `make` produce `codexion`; `make re` no deja basura; `make fclean` limpia el binario.

---

## LECCIÓN 18: M1 — Parsing y validación

**Estado: ✅ HECHO.**

Implementa: contar argumentos, convertir a números, validar (negativos, no enteros, scheduler), y guardarlos
en tu estructura de simulación.

Qué se consiguió:

- `main` comprueba que hay exactamente 9 tokens (`argc == 9`).
- `check_number(char **args)` valida **formato** de los 7 argumentos numéricos: todos los
  caracteres deben ser dígitos y debe haber al menos uno (`j == 0` detecta la cadena vacía).
  Rechaza `12a`, `1.5`, `-4`, `abc` y `""`.
- `check_scheduler(char **args)` exige exactamente `fifo` o `edf`.
- `parse_args` orquesta las dos anteriores y devuelve 0/1; `main` sale con código ≠ 0 si falla.
- Mensajes de error por `stderr` con `\n`. Norminette limpio.

Lo que **queda fuera de M1** a propósito:

- El rechazo por **valor** (p. ej. `number_of_coders == 0`) se decidirá al construir la `t_sim` (M2).
- El **desbordamiento** (`99999999999` pasa el formato) se detectará al convertir, no en `check_number`.
- Los números todavía **no se guardan**: no hay struct hasta M2.

**Pseudocódigo del pipeline de argumentos:**

```text
PSEUDO — parse_args(sim, argv)
    si check_number(argv) falla     → error "invalid argument"       → devolver 0
    si check_scheduler(argv) falla  → error "invalid argument"       → devolver 0
    fill_sim(sim, argv)             → copia los 8 valores a la t_sim
    si check_range(sim) falla       → error "invalid argument value" → devolver 0
    devolver 1
```

`check_number` mira el **formato** (solo dígitos, al menos uno) de los 7 numéricos; `check_range` mira
el **valor** (`n_coders > 0`, `compiles_required > 0`). Son dos preguntas distintas y por eso son dos
funciones.

- [x] Rechazo entradas inválidas sin leaks (`12a`, `""`, `-4`, `fifoX`, faltan args → `exit 1`).
- [ ] Guardo todos los parámetros en mi struct (**esto es parte de M2**).

---

## LECCIÓN 19: M2 — Estado compartido e inicialización

**Estado: ✅ HECHO.**

Reserva el array de dongles y de coders, inicializa mutex y cond de cada dongle, crea el mutex del log.
Define la convención izquierda/derecha.

Qué se consiguió (commits `f06142a`..`d1880cd`):

- `init.c` con `init_dongles`, `init_coders` (helpers `static`), `init` y `cleanup`.
- `malloc` **una vez** por array, bucles `i = 0` rellenan cada elemento (id 1..n, `left = dongles[i]`,
  `right = dongles[(i + 1) % n]` → el `%` da la vuelta a la mesa circular; con `n = 1`, left == right).
- Mutex por dongle + `stop_mutex` + `log_mutex` creados en `init`, destruidos en `cleanup`
  **antes** de los `free`, y `= NULL` al final.
- `main` llama a `parse_args → init → cleanup`; valgrind: `All heap blocks were freed`.
- Norminette OK en los 4 archivos. Herramientas instaladas en el equipo: `norminette` (uv) y `valgrind` (apt).

**Pseudocódigo del ciclo de vida:**

```text
PSEUDO — init_dongles(sim)
    dongles ← reservar n_coders × t_dongle
    si dongles == NULL → devolver 0
    para i de 0 a n_coders - 1:
        iniciar mutex y cond del dongle i
        is_free ← 1
        cooldown_ms ← cooldown que pidió el usuario
        cooldown_until ← 0
        heap ← reservar n_coders × t_request
        si heap == NULL → devolver 0
        heap_size ← 0
        heap_cap  ← n_coders
        arrival   ← 0
    devolver 1

PSEUDO — init_coders(sim)
    coders ← reservar n_coders × t_coder
    si coders == NULL → devolver 0
    para i de 0 a n_coders - 1:
        coder_id      ← i + 1
        compile_times ← 0
        last_compile  ← start_time      (el reloj arranca con la simulación, M7)
        sim           ← sim             (atajo para no pasarla por parámetro)
        left          ← &dongles[i]
        right         ← &dongles[(i + 1) % n_coders]
    devolver 1

PSEUDO — init(sim)
    si init_dongles(sim) falla → devolver 0
    start_time ← ahora en ms
    si init_coders(sim) falla  → cleanup(sim) ; devolver 0
    iniciar stop_mutex y log_mutex
    stop ← 0
    devolver 1

PSEUDO — cleanup(sim)
    para i de 0 a n_coders - 1:
        destruir mutex y cond del dongle i
        liberar heap del dongle i
    destruir stop_mutex y log_mutex
    liberar el array de dongles y el de coders
    dongles ← NULL ; coders ← NULL
```

La simetría es la garantía: **todo lo que `init_dongles`/`init_coders` crean, `cleanup` lo destruye
en orden inverso**, y `dongles` se libera después de sus heaps.

- [x] `init` crea todo y `cleanup` lo destruye simétricamente (valgrind limpio).

---

## LECCIÓN 20: M3 — Hilos y el ciclo del coder

**Estado: ✅ HECHO.**

Crea `n` hilos coder + 1 monitor. Cada coder, por ahora **sin dongles reales**, hace un ciclo
simple imprimiendo estados (con `usleep`), para verificar que arrancan y terminan bien.

**Lectura previa obligatoria:** Lección 1.2 y 1.3 (crear, esperar y por qué `join`).
Los conceptos ya los tienes: esto es cablearlos.

### Lo que ya está hecho

- `coder.c` con `coder_loop`: bucle compile→debug→refactor con `usleep`.
- `run_sim` en `main.c`: crea `n` hilos en un bucle, `join_threads` los espera.
- Comprobación del return de `pthread_create` en el camino de error (`!= 0`), con `join` de los
  hilos vivos antes de `cleanup`.
- `coder.c` añadido al `SRC` del Makefile.
- `make re` ✅, `norminette .` ✅, valgrind ✅ (0 leaks).

### Decisiones que tomaste (y por qué)

1. **La función del hilo** es `void *coder_loop(void *arg)`, recibe `&sim->coders[i]`.
2. **`pthread_create` devuelve 0 = bien.** El error es `!= 0` (ojo: al revés que tus `check_*`).
3. **`break` antes de `i++`** en el bucle de creación: así `i` = número de hilos que SÍ existen.
4. **`join` antes de `cleanup`**: nunca liberar `sim->coders` con hilos vivos usándolo.
5. **`join_threads(sim, i)`**: une solo los `i` vivos. En el caso normal `i == n`; en el error, `i < n`.

**Pseudocódigo de los hilos:**

```text
PSEUDO — coder_loop(arg)
    c ← arg
    mientras c.compile_times < compiles_required:
        acquire_dongles(c)
        last_compile ← ahora                (M7: marca el inicio de ESTA compilación)
        dormir comp_time ms
        compile_times ← compile_times + 1
        release_dongles(c)
        dormir debug_time ms
        dormir refactor_time ms
    terminar el hilo

PSEUDO — run_sim(sim)
    i ← 0
    mientras i < n_coders:
        si crear_hilo(coder_loop, &coders[i]) falla → salir del bucle
        i ← i + 1
    join_threads(sim, i)        (une solo los i que existen de verdad)
    (M8) crear el monitor y unirlo después

PSEUDO — join_threads(sim, n)
    para i de 0 a n - 1:
        esperar a que termine el hilo coders[i]
```

**Pseudocódigo objetivo de M8** (el `coder_loop` y el `run_sim` definitivos):

```text
PSEUDO — coder_loop, versión M8
    mientras NO sim_stopped(c.sim)  Y  c.compile_times < required:
        acquire_dongles(c)
        si sim_stopped(c.sim):
            release_dongles(c)
            terminar el hilo            (sin compilar ni contar)
        last_compile ← ahora
        dormir comp_time ms
        compile_times ← compile_times + 1
        release_dongles(c)
        dormir debug_time ms
        dormir refactor_time ms

PSEUDO — run_sim, versión M8
    crear los coders (parar al primer fallo; i = cuántos existen)
    si i == n_coders:
        mon_start ← (crear_hilo(monitor_loop, sim) == 0)
    join_threads(sim, i)                (primero los coders)
    si mon_start:
        esperar al monitor              (pthread_join)
```

### 🪤 `pthread_create` devuelve 0 = bien (y el `if` que se invierte solo)

Esta es **la confusión de M8**, así que va despacio. Todos los `pthread_*` que devuelven `int`
(`create`, `join`, `mutex_init`, `cond_init`…) siguen la convención POSIX:

```
0        → todo fue bien
≠ 0      → hubo un error (y el número dice cuál)
```

Para **detectar un fallo** se escribe `!= 0`:

```c
if (pthread_create(...) != 0)
	break ;
```

Pero cuando **guardas** el resultado, el nombre de la variable decide cómo debes comprobarlo. Hay dos
estilos válidos, y **no se pueden mezclar**:

| Estilo | Se guarda | Si no se intenta crear | Se comprueba |
|---|---|---|---|
| **booleano** | `(create(...) == 0)` → 1 si arrancó | queda `0` | `if (mon_start)` |
| **código crudo** | `create(...)` → 0 si arrancó | hay que poner **≠ 0** (p. ej. `-1`) | `if (mon_ret == 0)` |

⚠️ En el estilo crudo, si el hilo no llega a crearse la variable **no puede quedar a 0**: un `0`
significa “éxito” y el `if (mon_ret == 0)` haría un `join` inexistente. Por eso el estilo booleano
es más difícil de romper: su valor inicial (`0` = “no arrancó”) ya es el correcto.

Lo que **no** vale es el híbrido: guardar el código crudo en una variable llamada `mon_start` y
comprobarla con `if (mon_start)`. Eso significa “si **falló**, haz el join”:

| Caso | `mon_start = pthread_create(...)` | `mon_start = (pthread_create(...) == 0)` |
|---|---|---|
| se crea bien | `0` → `if (0)` falso → **no** hace join ❌ | `1` → `if (1)` → hace join ✅ |
| falla | `≠ 0` → **join de un hilo que no existe** ❌ | `0` → no hace join ✅ |
| no se intentó | queda `0` → no hace join | queda `0` → no hace join ✅ |

El diff exacto, con el resto de `run_sim` igual:

```diff
 	if (i == sim->n_coders)
-		mon_start = pthread_create(&sim->thread, NULL, monitor_loop, sim);
+		mon_start = (pthread_create(&sim->thread, NULL, monitor_loop, sim) == 0);
 	join_threads(sim, i);
 	if (mon_start)
 		pthread_join(sim->thread, NULL);
```

Otras tres decisiones de ese `run_sim` que también cuentan:

- **El monitor se crea después de los coders**, no antes: así no puede declarar burnout de hilos que
  aún no han nacido cuando `time_to_burnout` es muy pequeño.
- **Solo si `i == n_coders`**: si falló la creación de algún coder, el monitor acabaría inventándose
  un burnout de un coder inexistente.
- **Se une después de `join_threads`**: primero se vacían los coders (la cadena de `stop` los va
  soltando), y cuando el último sale, el monitor ya ha terminado o está a punto. Unirlo antes también
  funcionaría, pero este orden se lee mejor y deja claro quién depende de quién.

### ⚠️ Sobre el "interleaving"

Los 4 hilos salen **alineados por fase** (todos Compile, todos Debug...). **No es un bug:** ahora
mismo nada los frena, todos hacen el mismo `usleep` y arrancan casi a la vez. El entrelazado real
aparecerá en **M4**, cuando los dongles hagan esperar a unos y a otros.

### Pendiente que NO es de M3

- El monitor ya existe en `monitor.c` y `run_sim` lo lanza; falta que `coder_loop` consulte `stop` (M8).
- El `printf` da trazas; el log con formato exacto es M9.
- El `while (compile_times < required)` del coder cambiará en M8 (lo decide el monitor).

### Checkpoint (cumplido)

- [x] `make re` ✅ + `norminette .` ✅ + `valgrind --leak-check=full` ✅.
- [x] El ciclo compile→debug→refactor sale de todos los coders.
- [x] El programa termina solo (todos los hilos acaban y `main` sale) sin quedar colgado.

---

## LECCIÓN 21: M4 — Adquirir/soltar dongles (mutex + condition variable)

**Estado: ✅ HECHO.**

Añade el estado real de los dongles y la sección crítica: solo se compila teniendo los **dos**.
Aquí es donde blindas contra el **deadlock**.

### ✅ Resultado final (lo que quedó)

```c
void	take_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->is_free == 0)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	dongle->is_free = 0;
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_free = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
```

`coder_loop`: `take(left)` → `take(right)` → compile → `release(left)` → `release(right)` →
debug → refactor. Pruebas: 2 coders **entrelazados**, `5 coders` sin deadlock, valgrind 0 leaks.

### ⚠️ Dos correcciones que costaron (para no repetirlas)

| Error | Por qué es error |
|---|---|
| `pthread_mutex_lock(&dongle->is_free)` | `is_free` es un `int`; **el candado es `dongle->mutex`**. Se bloquea el mutex, no el dato |
| `pthread_cond_destroy(&sim->dongles->cond)` | `dongles->cond` es siempre `dongles[0].cond`. En bucle con `i`: `&sim->dongles[i].cond` |

### ✅ El "deadlock" de `take_dongle` (descartado)

No existe: el mutex serializa *mirar `is_free` + marcarlo*, así que **dos coders no pueden coger el
mismo dongle**. Y `cond_wait` **suelta el mutex mientras duerme** (si no, el que libera no podría
hacer `broadcast`). El deadlock real era el del **orden** de adquisición (siguiente punto).


### Punto de partida (lo que YA existe, no hay que crearlo)

| Ya existe | Dónde | Qué hace |
|---|---|---|
| `pthread_mutex_t mutex` dentro de `t_dongle` | `codexion.h` | **un mutex por dongle** (decisión tomada) |
| `pthread_cond_t cond` dentro de `t_dongle` | `codexion.h` | añadido en M4 — **falta inicializarlo y destruirlo** |
| `int is_free` | `t_dongle` | el estado del recurso (1 = libre) |
| `long cooldown_until` | `t_dongle` | para M5, ya reservado |
| `pthread_mutex_init` / `pthread_mutex_destroy` | `init.c` | creado en `init_dongles`, destruido en `cleanup` |
| `t_dongle *left` / `t_dongle *right` | `t_coder` | ya cableados: `left = dongles[i]`, `right = dongles[(i+1) % n]` |

**Lo que falta:** las funciones de coger/soltar y llamarlas desde `coder_loop`. Nada más.

### 🗺️ MAPA DE VARIABLES DE M4 (léelo antes de escribir una línea)

Que quede **clarísimo qué es cada cosa**. Todas estas variables aparecen en los ejemplos de abajo.

#### La confusión de base: `t_dongle` vs `t_dongle *`

| Se escribe | Qué es | Ejemplo |
|---|---|---|
| `t_dongle` | el **molde** (el tipo). Solo sirve para decir "de qué va el struct" | `malloc(sizeof(t_dongle) * n)` |
| `t_dongle *` | un **puntero** a un dongle: una dirección. Con él llegas al dongle **real** | `t_dongle *dongle;` |
| `t_dongle dongle` | una **copia** entera del struct, en la pila de la función | ⚠️ **esto es lo que tenías en `coder.c`** |

> ⚠️ **Por qué la copia es un error grave.** Al pasar `t_dongle` por valor, C copia el struct
> **entero** (mutex y cond incluidos) a la pila. Esa copia **no es** el dongle que ven los demás
> hilos: hacer `dongle.is_free = 0` dentro de la función **no cambia nada en la mesa**. Y copiar un
> `pthread_mutex_t` que está en uso es comportamiento indefinido.
>
> **Regla del proyecto:** todos los structs se pasan **por puntero**: `t_sim *`, `t_coder *`,
> `t_dongle *`. Si ves `t_sim sim` o `t_dongle dongle` en una firma, es un error.

#### La tabla grande

| Variable | Tipo | Dónde vive | Qué significa | Quién la escribe | Quién la lee |
|---|---|---|---|---|---|
| `sim` | `t_sim *` | parámetro de `take_dongle`/`release_dongle`; dentro del coder es `c->sim` | puntero a **la mesa**: la simulación entera (argumentos, arrays, flags, mutex del log) | `init` | todos los hilos |
| `c` | `t_coder *` | variable **local** de `coder_loop` | puntero a **"yo"**: esta persona concreta. Es el `arg` que le pasó `pthread_create` | `init_coders` | solo su hilo |
| `c->sim` | `t_sim *` | campo de `t_coder` | atajo para llegar a la mesa sin pasarla como parámetro | `init_coders` | su hilo |
| `c->left` | `t_dongle *` | campo de `t_coder` | **el dongle de mi izquierda** = `&dongles[i]` | `init_coders` | su hilo |
| `c->right` | `t_dongle *` | campo de `t_coder` | **el dongle de mi derecha** = `&dongles[(i+1) % n]` | `init_coders` | su hilo |
| `dongle` | `t_dongle *` | parámetro de `take_dongle`/`release_dongle` | puntero a **el dongle concreto** sobre el que trabajas ahora | — | quien llama |
| `dongle->mutex` | `pthread_mutex_t` | dentro de `t_dongle` | **el candado de ese dongle**; protege `is_free` | `init_dongles` | quien lo usa |
| `dongle->cond` | `pthread_cond_t` | dentro de `t_dongle` | **el timbre de ese dongle**; avisa a los que esperan | `init_dongles` | quien espera |
| `dongle->is_free` | `int` | dentro de `t_dongle` | **el estado**: `1` = libre, `0` = alguien lo tiene | `take_dongle` (→0) y `release_dongle` (→1) | `take_dongle`, para decidir si espera |
| `dongle->cooldown_until` | `long` | dentro de `t_dongle` | instante (ms) hasta el que el dongle está en cooldown. **M5**, todavía quieto | `release_dongle` (M5) | M5 |
| `c->compile_times` | `int` | dentro de `t_coder` | nº de compilaciones que **yo** ya he hecho | mi propio hilo | mi hilo (+ monitor en M8) |
| `c->last_compile` | `long` | dentro de `t_coder` | ms desde el inicio en los que **yo** empecé mi última compilación | mi propio hilo | monitor (M8) |
| `sim->log_mutex` | `pthread_mutex_t` | dentro de `t_sim` | candado del **log** (M9) | `init` | todos (M9) |

#### El dibujo de la mesa

```
sim  (t_sim *)  ─────────────────────────────►  la mesa: la comparten TODOS los hilos
 │
 ├── coders ──▶ [ coder 0 ][ coder 1 ][ coder 2 ]
 │                │         │         │
 │              c->left   c->left   c->left     (t_dongle *)
 │              c->right  c->right  c->right
 │               d0,d1    d1,d2    d2,d0
 │
 ├── dongles ─▶ [ dongle 0 ][ dongle 1 ][ dongle 2 ]   (t_dongle)
 │                │  mutex   │  mutex   │  mutex      ← un candado POR dongle
 │                │  cond    │  cond    │  cond       ← un timbre POR dongle
 │                │  is_free │  is_free │  is_free    ← el estado que se protege
 │
 └── stop_mutex, log_mutex
```

#### Los tres `&` que hay que entender bien

| Se escribe | Significado | Dónde se usa |
|---|---|---|
| `&dongle->mutex` | "la dirección del mutex que está **dentro** del dongle" | `pthread_mutex_lock(&dongle->mutex)` |
| `&sim->dongles[i].cond` | "la dirección de la cond que está dentro del dongle nº `i`" | `pthread_cond_init(&sim->dongles[i].cond, NULL)` |
| `&sim->coders[i].left` | "la dirección del **puntero** `left` que está dentro del coder nº `i`" | cuando le asignas el dongle |

> **Regla:** el `&` va delante de una **variable** (un objeto). Nunca delante de algo que ya es una
> dirección: `&dongle->mutex` sí (el mutex es un objeto), pero `&c->left` **tampoco** — `c->left`
> ya es un puntero, y `&c->left` sería "la dirección del puntero", que es otra cosa distinta.

### Decisión 1: un mutex por dongle (no uno global)

El usuario lo razonó así: *"uno para cada dongle porque se liberan y se comprometen a tiempos
distintos"*. Correcto. Con un solo mutex global, para mirar el dongle 3 bloqueas también el 0, el 1,
el 2... todo el mundo en cola por cualquier dongle. Con uno por dongle, cada conflicto es local.

### Decisión 2: condition variable, NO busy-wait

El usuario eligió **condition variable** y dio el argumento exacto:

- El **busy-wait** gasta CPU y el `usleep` que lo acompaña es impreciso: "puede pasarse o quedarse
  corto". No es justo y no es fiable.
- El **condition variable** duerme de verdad (0% CPU mientras espera) y despierta justo cuando
  alguien avisa.

Y un motivo extra que decidiste sin saberlo: **el cooldown (M5) también necesita espera.** Con `cond`
esa espera encaja de forma natural (`pthread_cond_timedwait`); con busy-wait habría que rehacerlo.
Elegir cond ahora ahorra trabajo en M5.

**Las tres piezas y para qué sirve cada una** (esto es lo que no cuadraba en los ejemplos):

```
t_dongle / is_free      → el ESTADO del recurso   (¿lo tiene alguien?)
pthread_mutex_t mutex   → PROTEGE ese estado      (que se mire de uno en uno)
pthread_cond_t  cond    → CANAL de aviso          (para no preguntar en bucle)
```

La condición **no guarda nada**. Es solo el timbre: "quien espere aquí, que se despierte y **vuelva
a mirar**". El estado sigue en `is_free`. Por eso la condición siempre va con el mutex: uno protege,
el otro avisa.

### El patrón obligatorio — y qué es cada línea

Primero la versión abstracta (la idea):

```
lock(mutex)
while (!puedo)              ← si esto no se cumple, vuelvo a esperar
    cond_wait(cond, mutex)  ← suelta el mutex y duerme; al despertar lo recupera
hago la acción              ← ya tengo el mutex Y la condición se cumple
unlock(mutex)
```

Ahora la versión **con los nombres reales de tu proyecto** (esto es lo que te faltaba: saber qué es
cada cosa):

```c
void	take_dongle(t_sim *sim, t_dongle *dongle)
{
	(void)sim;
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->is_free == 0)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	dongle->is_free = 0;
	pthread_mutex_unlock(&dongle->mutex);
}
```

| Línea | Qué hace | Por qué |
|---|---|---|
| `void take_dongle(t_sim *sim, t_dongle *dongle)` | declara la función | `sim` y `dongle` son **punteros**: así tocas el dongle de verdad, no una copia |
| `(void)sim;` | "este parámetro no lo uso (todavía)" | con `-Werror`, un parámetro sin usar **rompe la compilación** |
| `pthread_mutex_lock(&dongle->mutex);` | cojo el candado **de ese dongle** | `&` porque `mutex` es un objeto dentro del struct; y el candado es el del dongle, no uno global |
| `while (dongle->is_free == 0)` | mientras esté ocupado, no puedo cogerlo | **`while`, no `if`**: al despertar puede que otro se lo haya llevado |
| `pthread_cond_wait(&dongle->cond, &dongle->mutex);` | duerme hasta que le avisen | suelta el mutex mientras duerme y lo **vuelve a coger** al despertar |
| `dongle->is_free = 0;` | **toma** el dongle | solo puede hacerse teniendo el mutex → nadie más puede hacerlo a la vez |
| `pthread_mutex_unlock(&dongle->mutex);` | suelta el candado | **después** de tocar el estado, nunca antes |

Y la pareja, la otra mitad de la sección crítica:

```c
void	release_dongle(t_sim *sim, t_dongle *dongle)
{
	(void)sim;
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_free = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
```

| Línea | Qué hace | Por qué |
|---|---|---|
| `pthread_mutex_lock(&dongle->mutex);` | coge el candado | sin esto, `is_free = 1` y `broadcast` podrían pisarse con otro hilo |
| `dongle->is_free = 1;` | devuelve el dongle a la mesa | el estado **cambia aquí**, protegido por el mutex |
| `pthread_cond_broadcast(&dongle->cond);` | **toca el timbre**: "hay dongle libre, despierten los que esperan" | `broadcast` y no `signal`: pueden esperar varios en el mismo dongle, y con `signal` solo se despertaría uno (puede que no sea el que le toca) |
| `pthread_mutex_unlock(&dongle->mutex);` | suelta el candado | |

Y una tercera clave, la que se te va a colar: **el `broadcast` va ANTES del `unlock`.** Si avisas
después de soltar el candado, otro hilo puede despertar, no encontrar el dongle libre y volver a
dormirse **sin que nadie lo vuelva a avisar** → el timbre queda mudo.

### 🔴 Decisión 3: el orden de adquisición (CORREGIDA — la anterior provocaba un cuelgue)

> **Aviso.** La versión anterior de esta guía decía *"todos cogen primero el izquierdo, después el
> derecho; así no hay ciclo"*. **Es falso**, y era la idea que tenías en la cabeza. Con `n = 4` eso
> **cuelga el programa**. Aquí está la traza, paso a paso.

#### La traza del deadlock (n = 4, los 4 arrancan a la vez)

Convención del proyecto: el coder de índice `i` (0-based) coge `left = dongles[i]` y
`right = dongles[(i+1) % 4]`.

```
Coder 0:  left = d0   right = d1
Coder 1:  left = d1   right = d2
Coder 2:  left = d2   right = d3
Coder 3:  left = d3   right = d0      ← este es el que cierra el círculo
```

Si todos cogen primero el **izquierdo**, los 4 dongles se reparten sin conflicto (son distintos):

```
t0:  c0 toma d0    c1 toma d1    c2 toma d2    c3 toma d3
t1:  c0 pide d1 → OCUPADO     c1 pide d2 → OCUPADO
     c2 pide d3 → OCUPADO     c3 pide d0 → OCUPADO
t2:  los 4 esperando. NINGUNO tiene sus dos dongles.
     → nadie puede soltar nada. El programa se cuelga PARA SIEMPRE.
```

La espera es circular: `c0 → d1 → c1 → d2 → c2 → d3 → c3 → d0 → c0`.
Se cumplen **las 4 condiciones de Coffman**:

| Condición de Coffman | ¿Se cumple aquí? |
|---|---|
| Exclusión mutua | un dongle, un dueño ✔ |
| Retención y espera | c0 **ya tiene** d0 y **espera** d1 ✔ |
| Sin expropiación | a c0 no le quitan d0 ✔ |
| **Espera circular** | d0 → d1 → d2 → d3 → d0 ✔ |

#### ✅ La regla correcta: orden global por índice

> **Cada coder coge sus dos dongles en orden CRECIENTE de índice: primero el de índice MENOR,
> después el de índice MAYOR.**

| Coder | sus dos dongles | orden correcto | ¿coincide con "izquierdo primero"? |
|---|---|---|---|
| 0 | d0, d1 | d0 → d1 | sí |
| 1 | d1, d2 | d1 → d2 | sí |
| 2 | d2, d3 | d2 → d3 | sí |
| 3 (el último) | d3, d0 | **d0 → d3** | **NO** → coge primero su *derecho* |

Es decir: **todos menos el último cogen el izquierdo primero; el último coge el derecho primero.**
No es una chapuza: es **ordenar los recursos con un orden global** (aquí, el índice del dongle) y
que cada hilo los coja **siempre** en ese orden. Eso **rompe la espera circular** por construcción.

#### Traza con la regla correcta (n = 4)

```
t0:  c0 pide d0 → LIBRE → lo tiene      c3 pide d0 → OCUPADO (lo tiene c0) → espera
t1:  c1 pide d1 → LIBRE → lo tiene      c0 pide d1 → OCUPADO (lo tiene c1) → espera
t2:  c2 pide d2 → LIBRE → lo tiene      c1 pide d2 → OCUPADO (lo tiene c2) → espera
t3:  c2 pide d3 → LIBRE → lo tiene → c2 ya tiene d2 y d3 → compila → suelta
t4:  c1 pide d2 → ahora LIBRE → lo tiene → compila → suelta
t5:  c0 pide d1 → ahora LIBRE → lo tiene → compila → suelta
t6:  c3 pide d0 → ahora LIBRE → lo tiene → pide d3 → LIBRE → lo tiene → compila
```

✅ **Nadie espera en ciclo.** La cadena de esperas arranca en el que espera el índice más bajo
(d0) y **termina** en el que puede completarse (d3).

> **El invariante que hay que repetirse (y que va en el README):** con este orden, un hilo que
> espera **siempre espera un índice MAYOR que todos los que ya tiene**. El que tiene el índice más
> alto nunca está esperando nada → el último de la cadena siempre puede terminar, soltar, y desbloquear
> a todos los demás. Por eso no puede haber ciclo **en ningún interlineado**, no solo en el de arriba.

#### Cómo se implementa el "menor índice primero"

Tus dos dongles son `c->left` y `c->right`, y **los dos son punteros dentro del MISMO array**
(`sim->dongles`). En C, comparar dos punteros **del mismo array** con `<` o `>` **es comparar sus
índices**: lo garantiza el estándar, siempre que no se comparen punteros de arrays distintos.

```c
t_dongle	*first;
t_dongle	*second;

first = c->left;
second = c->right;
if (first > second)
{
	first = c->right;
	second = c->left;
}
take_dongle(c->sim, first);     /* el de índice MENOR: el primero que se coge */
take_dongle(c->sim, second);    /* el de índice MAYOR: el segundo */
```

> ⚠️ **Trampa:** comparar punteros de **arrays distintos** no significa nada (el estándar no lo
> define). Aquí es seguro **porque** `left` y `right` salen los dos de `sim->dongles`. Si algún día
> cambiabas la disposición de la mesa, esta comparación dejaría de ser válida → la alternativa
> blindada es guardar los **índices** en el `t_coder` en vez de los punteros.

#### 🔴 El caso `n = 1`: `left` y `right` son EL MISMO dongle

Con `n = 1`: `left = &dongles[0]` y `right = &dongles[(0 + 1) % 1] = &dongles[0]`.
**Son el mismo dongle.** Si haces `take_dongle(left)` y después `take_dongle(right)`, el segundo
`take_dongle` ve `is_free == 0` (lo acabas de tomar tú) y **se espera a sí mismo → cuelgue eterno**.

```
c0: take_dongle(d0) → is_free = 0, vale
c0: take_dongle(d0) → is_free == 0 → espera… espera… (él mismo es el dueño; nadie va a soltarlo)
    → CUEGUE
```

El subject dice que con una persona hay **un solo dongle** sobre la mesa → se coge **una vez**:

```c
take_dongle(c->sim, first);
if (first != second)
	take_dongle(c->sim, second);
...
release_dongle(c->sim, first);
if (first != second)
	release_dongle(c->sim, second);
```

> `first != second` compara **punteros**: si son el mismo dongle, dan la misma dirección → son
> iguales. Funciona porque los dos punteros salen del mismo array. (Y si `n > 1`, `first` y `second`
> son dongles distintos → la condición se cumple y coge los dos.)


#### La otra opción (la necesitarás en M6): un árbitro central

| | **Opción A — orden por índice** (la de arriba) | **Opción B — árbitro central** |
|---|---|---|
| Idea | cada coder coge sus dongles en un orden global fijo; si no puede, espera | un mutex + una cola en `t_sim`: el coder **pide el par** y el árbitro lo concede cuando puede |
| Deadlock | imposible **por el orden global** | imposible porque **nadie coge medio par**: se conceden los 2 a la vez |
| Coste ahora | nada: es ordenar dos punteros | un mutex nuevo, una cola de peticiones, la lógica de concesión |
| ¿Sirve para FIFO/EDF (M6/M7)? | ❌ no: no hay ningún sitio donde decidir "quién va primero" | ✅ sí: la cola de peticiones **es** el sitio donde FIFO/EDF deciden |
| Riesgo | el orden hay que respetarlo en **todas** las adquisiciones | más código de golpe |

**Recomendación para M4:** Opción A (es lo mínimo que cumple el hito y te hace ver a Coffman en
práctica). Si en M6 el heap te obliga a rehacer el reparto, ese es el precio de entender por qué
existe un árbitro. La Opción B la escribirás igualmente en M6/M7.

⚠️ Es fácil confundir esta pregunta con *"¿quién gana si dos quieren el mismo dongle?"*: eso es el
scheduler (M6 FIFO / M7 EDF) y **no** se resuelve aquí.

### Diseño: el `coder_loop` completo, línea por línea

Prefiere **una función que coge un dongle y otra que lo suelta** — tonta cada una, una sola cosa.
Y como la Norm limita cada función a **25 líneas**, el par de adquisiciones va en su propio helper:

```c
static void	acquire_dongles(t_coder *c)
{
	t_dongle	*first;
	t_dongle	*second;

	first = c->left;
	second = c->right;
	if (first > second)
	{
		first = c->right;
		second = c->left;
	}
	take_dongle(c->sim, first);
	if (first != second)
		take_dongle(c->sim, second);
}

static void	release_dongles(t_coder *c)
{
	release_dongle(c->sim, c->left);
	if (c->left != c->right)
		release_dongle(c->sim, c->right);
}

void	*coder_loop(void *arg)
{
	t_coder	*c;

	c = arg;
	while (c->compile_times < c->sim->compiles_required)
	{
		acquire_dongles(c);
		printf("Compile\n");
		usleep(c->sim->comp_time * 1000);
		release_dongles(c);
		c->compile_times += 1;
		printf("Debug\n");
		usleep(c->sim->debug_time * 1000);
		printf("Refactor\n");
		usleep(c->sim->refactor_time * 1000);
	}
	return (NULL);
}
```

> 📌 **Nota de evolución (M4 → hoy).** Este bloque es el `coder_loop` de M4, tal como se escribió.
> Después: `take_dongle` / `release_dongle` reciben **el coder** (`t_coder *`) para poder consultar
> `sim->scheduler`, el heap y el cooldown; los `printf` de estado pasan por el log serializado (M9);
> y el `while` gana `!sim_stopped(c->sim)` en M8.

| Pieza | Qué hace | Trampa que evita |
|---|---|---|
| `t_dongle *first; t_dongle *second;` | dos punteros **locales**, al principio del bloque | declararlos a mitad de bloque incumple la Norm; y sin declararlos **no compila** |
| `if (first > second)` → reordenar | **orden global por índice** de Decisión 3 | "izquierdo primero" **cuelga con n ≥ 3** |
| `if (first != second) take_dongle(second)` | solo coge el segundo si son **distintos** | con `n = 1` los dos son el mismo dongle → **se esperaría a sí mismo** |
| `release_dongles(c)` justo tras el `usleep` de compilar | los suelta **antes** de depurar y refactorizar | tener dongles en debug/refactor hambrienta a los vecinos → burnout |
| `c->compile_times += 1;` **después** de soltar | se cuenta al terminar, no al empezar | — |
| `* 1000` en los `usleep` | los argumentos van en **ms** y `usleep` pide **µs** | compilaría 1000 veces más rápido de lo que dice el usuario |

> **El orden en `coder_loop` importa:** `acquire` → compilar → `release` → contar → debug → refactor →
> vuelta a empezar. `printf("Compile")` va **dentro** del par, porque el log del subject pide dos
> `has taken a dongle` antes de `is compiling` (la traducción a ese formato es M9).
>
> **Por qué NO meter "¿cuántos tengo?" dentro de `take_dongle`:** mezclaría estado y condiciones donde
> no tocan. `take_dongle` coge **uno** y punto. Coger dos es responsabilidad del coder (aquí, de
> `acquire_dongles`).

> 📐 **Orden de definición en `coder.c`** (los `static` se declaran antes de usarse):
> `take_dongle` → `release_dongle` → `acquire_dongles` → `release_dongles` → `coder_loop`.
> Así `coder.c` se queda en **exactamente 5 funciones**, el límite de la Norm.

**Pseudocódigo (así está hoy, tras M6):**

```text
PSEUDO — take_dongle(coder, dongle)
    bloquear el mutex del dongle
    apuntar mi petición en el heap de ese dongle
        seq ← arrival del dongle ; arrival ← arrival + 1
        key ← seq (fifo) o deadline (edf)
    mientras no sea la raíz del heap
            o el dongle esté ocupado
            o el dongle siga en cooldown:
        si no soy la raíz o está ocupado → esperar en la cond, sin límite
        si no (solo falta cooldown)      → esperar con temporizador hasta cooldown_until
    sacar la raíz del heap          (es mi petición)
    is_free ← 0
    desbloquear el mutex

PSEUDO — release_dongle(dongle)
    bloquear el mutex
    cooldown_until ← ahora + cooldown_ms
    is_free ← 1
    despertar a todos los que esperan en esa cond
    desbloquear el mutex

PSEUDO — acquire_dongles(c)
    first  ← el de índice MENOR entre left y right
    second ← el otro
    take_dongle(c, first)
    si first != second → take_dongle(c, second)

PSEUDO — release_dongles(c)
    release_dongle(c, left)
    si left != right → release_dongle(c, right)
```

### Cambios en struct y ciclo de vida

1. `t_dongle` → añadir `pthread_cond_t cond;` ✅ (ya está hecho).
2. `init_dongles` → `pthread_cond_init(&sim->dongles[i].cond, NULL);` junto al `pthread_mutex_init`.
3. `cleanup` → `pthread_cond_destroy(&sim->dongles[i].cond);` dentro del bucle, junto al `destroy`
   del mutex.
4. **Prototipos en `codexion.h`** para `take_dongle` y `release_dongle` (hoy no están: la Norm y
   el compilador los exigen).

```c
void	take_dongle(t_sim *sim, t_dongle *dongle);
void	release_dongle(t_sim *sim, t_dongle *dongle);
```

> **Alineación de prototipos:** `void` son 4 letras → **1 tab** (como `cleanup`, `join_threads`...).

### Preguntas antes de escribir

1. ¿Por qué `while` y no `if` alrededor de `cond_wait`?
2. ¿Por qué `broadcast` y no `signal` cuando varios esperan el mismo dongle?
3. ¿Qué condición de Coffman **rompe** el orden por índice, y por qué "izquierdo primero" no la
   rompe?
4. Con `n = 1`, ¿qué dos nombres son iguales y qué pasa si llamas dos veces a `take_dongle`?
5. ¿Por qué el parámetro es `t_dongle *` y no `t_dongle`?

### Checkpoint

- [ ] `make re` vuelve a estar **verde** (los errores de `init.c` y de `coder.c` resueltos).
- [ ] Dos coders no cogen nunca el mismo dongle a la vez, y con **2 coders no se cuelga**.
- [ ] Con `n = 4` **no hay deadlock**: los 4 terminan. (Si cuelga, mira la traza de Decisión 3.)
- [ ] Con `n = 1` termina (no se espera a sí mismo).
- [ ] `norminette coders/` a 0 + `valgrind --leak-check=full` sin fugas.

---

## LECCIÓN 22: M5 — Cooldown

**Estado: ✅ HECHO.**

Un dongle, al soltarse, **no puede volver a cogerse** hasta que pase `dongle_cooldown` ms. Si no
existe cooldown, dos coders podrían turnarse el dongle instantáneamente y el de en medio se muere
de hambre.

### Cómo quedó

| Pieza | Qué hace |
|---|---|
| `t_dongle.cooldown_ms` | copia de `sim->dongle_cooldown`, puesta en `init_dongles` |
| `t_dongle.cooldown_until` | instante **absoluto** (epoch ms) hasta el que no se puede coger |
| `get_time_ms()` (`time.c`) | `gettimeofday` → epoch en ms |
| `wait_until_ms()` (`time.c`) | `ms_to_timespec` + `pthread_cond_timedwait` |
| `release_dongle` | `cooldown_until = get_time_ms() + cooldown_ms` **antes** de `is_free = 1` |
| `take_dongle` | `while (is_free == 0 \|\| get_time_ms() < cooldown_until)` |

Dentro del `while` hay **dos esperas distintas**, y ese es el matiz:

```
si is_free == 0            → pthread_cond_wait        (sin límite: que me avisen al soltar)
si no, pero en cooldown    → wait_until_ms(until)     (con límite: se acaba solo)
```

### ⚠️ La trampa que acertaste

`pthread_cond_timedwait` pide una hora **ABSOLUTA** (desde 1970), no "dentro de 50 ms". Por eso
`cooldown_until` guarda epoch-ms y `ms_to_timespec` lo divide en segundos/nanosegundos. Si hubieras
guardado el cooldown como "50 ms a partir de ahora", `timedwait` habría vencido en 1970 y volvería
al instante.

También tiene que ser el **mismo reloj**: `gettimeofday` = `CLOCK_REALTIME` = el que usa
`pthread_cond_timedwait` por defecto. Si mezclas `CLOCK_MONOTONIC` con `gettimeofday`, los números no
significan lo mismo.

### ✅ Verificación (la que faltaba)

Mismo escenario, cambiando solo el cooldown:

| `dongle_cooldown` | Duración |
|---|---|
| 0 ms | 0.14 s |
| 200 ms | 1.14 s |
| 300 ms (n=4) | 2.92 s |

Si el cooldown no se aplicara, el tiempo **no cambiaría**. Escala ⇒ funciona.

**Pseudocódigo del cooldown en el tiempo** (`dongle_cooldown = 200`, se suelta en el instante 1000):

```text
instante   1000                1200      1201
           |-------------------|---------> tiempo
           suelta              ya cogible
           is_free = 1 desde 1000
           cooldown_until = 1200

un coder que llega en 1100:
    is_free == 1            → no duerme en la cond
    ahora < cooldown_until  → duerme con temporizador hasta 1200
    al despertar vuelve a comprobar el mientras (nunca se fía del reloj)
```

La clave: `is_free = 1` y "se puede coger" **no** son lo mismo durante el cooldown.

### Checkpoint

- [x] Tras soltar, el dongle no se vuelve a coger antes de `dongle_cooldown` ms.
- [x] Sin cuelgues con cooldown > 0 (`n=6`, `cooldown=50` × 15 iteraciones: 0 fallos).
- [x] valgrind limpio.

---

## LECCIÓN 23: M6 — El heap y la planificación FIFO

**Estado: ✅ CERRADO.** Heap verificado aislado y raíz respetada en `take_dongle`.

Antes de M6, `take_dongle` **no elegía**: cuando el dongle estaba libre, el primero que despertaba
se lo llevaba (el que el SO eligiera). Eso **no es justo** y el subject exige un reparto explícito.
Ahora el reparto lo decide el heap.

### La decisión de diseño (cerrada, no se reabre)

Se mantiene **todo** lo de M4 y solo se añade una pieza:

| Se queda tal cual | Se añade |
|---|---|
| mutex + cond **por dongle** | un **heap de peticiones por dongle** |
| `acquire_dongles` con orden por índice | `arrival` por dongle (contador de llegada) |
| `release_dongles` y `coder_loop` | `heap.c` con 5 funciones |
| `release_dongle` **sin ni una línea de cambio** | la condición "soy la raíz" en `take_dongle` |

**Descartado:** árbitro central en `t_sim` y concesión de pares. No hacen falta. El orden por índice
ya mata el deadlock, y FIFO/EDF solo tienen que decidir **a quién se le da un dongle cuando hay
varios esperándolo**. Menos piezas, menos sitios donde equivocarse.

### Quién decide: el root se sirve a sí mismo

No hay nadie repartiendo desde fuera. Cada coder, al pedir un dongle:

1. Se apunta en el heap de ese dongle con `seq = d->arrival++` (`key = seq` en M6).
2. Espera mientras **no sea la raíz** del heap, o el dongle esté ocupado, o esté en cooldown.
3. Cuando es la raíz, está libre y sin cooldown → `heap_pop` y se lo queda.

`release_dongle` solo marca `is_free = 1` y hace `broadcast`. Los que esperan se despiertan,
recalculan, y **solo el root** puede pasar. Eso *es* FIFO.
**Pseudocódigo del heap:**

```text
PSEUDO — request_less(a, b)
    si a.key < b.key  → sí (a va antes)
    si a.key == b.key → decide el desempate: a.seq < b.seq
    si no             → no

PSEUDO — sift_up(d, i)
    mientras i > 0:
        padre ← (i - 1) / 2
        si heap[i] no es menor que heap[padre] → parar
        intercambiar heap[i] y heap[padre]
        i ← padre

PSEUDO — sift_down(d, i)
    mientras exista hijo izquierdo de i:
        elegir el menor de los dos hijos (si hay derecho)
        si el elegido no es menor que heap[i] → parar
        intercambiar heap[i] con el elegido
        i ← índice del elegido

PSEUDO — heap_push(d, r)
    si size >= capacidad → no hacer nada (no cabe)
    heap[size] ← r                  (size es la PRIMERA casilla libre)
    size ← size + 1
    sift_up(d, size - 1)

PSEUDO — heap_pop(d)
    top ← heap[0]
    size ← size - 1                 (bajar el tamaño PRIMERO)
    heap[0] ← heap[size]            (el último pasa a la raíz)
    sift_down(d, 0)
    devolver top
```

Simetría que hay que memorizar: `push` escribe en `size` **antes** de subir el tamaño; `pop` lee
`size - 1` **después** de bajarlo. Las dos veces es "la última casilla ocupada".



**Las dos esperas siguen siendo distintas** (esto es lo que hay que clavar):

| Situación | Espera |
|---|---|
| no soy root, o el dongle está ocupado | `cond_wait` **sin límite** |
| soy root, libre, pero en cooldown | `wait_until_ms` (`timedwait`) |

Si soy root puedo dormir hasta que expire el cooldown sin miedo. Si **no** soy root, un `timedwait`
sería un bucle inútil: nadie me va a dar el turno por reloj, solo cuando salga el de delante.

### 🪤 El bug que casi cuela: `heap_pop` sin comprobar la raíz

Si `take_dongle` hace `heap_pop` solo con "el dongle está libre":

```
A se apunta (root). B se apunta detrás.
El dongle está libre → B no espera.
B hace heap_pop → se lleva la petición de A, no la suya.
B toma el dongle. A espera para siempre en un heap del que ya lo han borrado.
```

No es solo injusto: **se pierden peticiones**. El `heap_pop` va **después** del `while`, y el `while`
incluye la condición de raíz.

### Por qué no hace falta tocar el orden de adquisición

Quien espera un dongle solo tiene dongles de **índice menor** (invariante de M4). El heap no cambia
*qué* se pide ni *en qué orden*, solo *a quién* se le concede. Un ciclo de espera seguiría exigiendo
una cadena de índices que sube y vuelve a bajar → imposible.

### Preguntas antes de escribir código

1. En un heap de mínimos, ¿qué operaciones necesitas y cuánto cuestan? (`push`, `pop`, `peek`)
2. Con `fifo`, ¿qué clave usas? ¿Por qué `d->arrival++` y no `get_time_ms()`?
3. ¿Por qué el comparador mira **dos** campos (`key` y `seq`) y no uno?
4. ¿Qué pasa si `heap_pop` se ejecuta sin comprobar antes que eres la raíz?

### Checkpoint

- [x] `heap.c` probado **aislado** (12 valores desordenados → salen ordenados + empate por `seq`).
- [x] Con muchos coders y `fifo`, se respeta el orden de llegada (raíz del heap).
- [x] 0 cuelgues en el estrés de `n = 1..8` (×10) y `n = 12` `edf` (×10).
- [x] El cooldown sigue escalando igual que en M5 (0.14 s → 1.14 s).
- [x] valgrind: los heaps de los dongles se liberan (`0 bytes lost`).

### 🪤 Errores reales de este hito (para no repetirlos)

| Error | Síntoma | Regla |
|---|---|---|
| `heap_pop` leía `heap[size]` antes de `size--` | basura en la raíz, se perdía el último | primero `size--`, luego `heap[size]` |
| `init_dongles` no reservaba el heap | SIGSEGV al primer `heap_push` | cada `malloc` de una pieza del dongle, en `init_dongles` |
| `malloc` del heap sin `if == NULL` | escribir en `NULL` si falta memoria | el check va **justo debajo** del `malloc` |
| check del `malloc` puesto **antes** del `malloc` | `exit=1` mudo (leía memoria sin inicializar) | orden: `malloc` → check → usar |
| `while` del bucle con un tab más | `-Wmisleading-indentation` = error con `-Werror` | un tab por nivel, sin excepciones |
| `if` interno del `while` solo miraba `is_free` | busy-wait al 100 % de CPU | el `if` pregunta **lo mismo** que el `while` menos el cooldown |


---

## LECCIÓN 24: M7 — Planificación EDF

**Estado: ✅ CERRADO.**

EDF = *Earliest Deadline First*. En FIFO la clave es **cuándo llegaste**; en EDF es **cuándo te
agotas**. El que tiene el burnout más cerca va primero, aunque acabe de llegar.

```
deadline(coder) = last_compile + time_to_burnout
```

`last_compile` es el instante en que **empezó** su última compilación. Ojo: la muerte se mide desde
que empieza a compilar, no desde que termina (el subject: *no debe pasar más de `time_to_burnout`
desde el inicio de su última compilación*).

### Lo bueno: el heap ya lo soporta

No se toca `heap.c`, ni `sift_up`, ni `sift_down`, ni `request_less`. Solo cambia **de dónde sale
`key`**. Eso es exactamente por lo que el heap guarda `key` como campo aparte de `seq`:

| Campo | Para qué |
|---|---|
| `key` | el criterio del scheduler (`seq` en FIFO, `deadline` en EDF) |
| `seq` | desempate estable cuando dos peticiones tienen la misma `key` |

### Los tres cambios

**1. `init_coders`** — `last_compile` no puede quedarse a `0`:

Si vale 0, `deadline = 0 + time_to_burnout` = epoch + 1000 ms, es decir **el año 1970**: todos los
coders tendrían la misma clave ridícula y EDF degeneraría en "lo que salga". Hay que arrancar el
reloj de cada coder al empezar la simulación:

```c
sim->start_time = get_time_ms();      /* en init(), antes de init_coders */
...
sim->coders[i].last_compile = sim->start_time;
```

(`start_time` ya está declarado en `t_sim` y nunca se ha usado: este es su momento.)

**2. `coder_loop`** — marcar el inicio de la compilación:

```c
while (...)
{
	acquire_dongles(c);
	c->last_compile = get_time_ms();   /* ← justo al empezar a compilar */
	usleep(c->sim->comp_time * 1000);
	...
}
```

Va **después** de `acquire_dongles` y **antes** del `usleep`, porque el reloj de burnout empieza
cuando la compilación empieza de verdad, no mientras se esperaba el dongle.

**3. `take_dongle`** — la clave según el scheduler:

```c
req.coder = coder;
req.seq = dongle->arrival++;
if (strcmp(coder->sim->scheduler, "edf") == 0)
	req.key = coder->last_compile + coder->sim->time_to_burnout;
else
	req.key = req.seq;
heap_push(dongle, req);
```

`strcmp` está en la lista de funciones permitidas. El `else` deja FIFO exactamente como está hoy.

**Pseudocódigo de la elección de clave:**

```text
PSEUDO — take_dongle(coder, dongle), solo la clave
    seq ← arrival del dongle
    arrival ← arrival + 1
    si scheduler == "edf":
        key ← last_compile + time_to_burnout      (cuándo se agota)
    si no:
        key ← seq                                 (cuándo llegó)

PSEUDO — coder_loop, solo el reloj
    acquire_dongles(c)
    last_compile ← ahora        (desde aquí cuenta el próximo burnout)
    dormir comp_time ms
```

Ejemplo con `time_to_burnout = 100`, dos coders que piden a la vez:

| Coder | `last_compile` | `key` (deadline) | ¿Quién pasa antes? |
|---|---|---|---|
| A | 1000 | 1100 | **A** (se agota antes) |
| B | 1050 | 1150 | B |

Y si B acaba de compilar, su `last_compile` es más nuevo → su deadline más lejano → **se hunde solo**
en el heap. No hay ninguna decisión de "dar prioridad": el heap ordena por `key`.

### 🪤 El detalle que parece que no hace nada

Con `seq`, la clave **crece** con el tiempo (0, 1, 2, 3...). Con `deadline`, la clave es un instante
absoluto (epoch en ms, del orden de 1,7 × 10¹²). Los dos son `long` y el heap no nota la diferencia:
solo compara. Por eso no hay que tocar nada más.

Y un coder recién compilado tiene `last_compile` **grande** → deadline grande → **va al final**. Su
petición se hunde en el heap sola. Un coder que lleva mucho sin compilar tiene deadline pequeño → se
sube a la raíz solo. El heap *es* el planificador.

### Cómo se prueba (M8 aún no existe)

El monitor todavía no detecta burnout, así que **no se puede comprobar "nadie se agota" con la
salida del programa**. Dos formas honestas de verificar M7 ahora:

1. **Traza temporal** en `take_dongle`: imprimir `coder_id` y `key` al hacer `heap_pop`, y comprobar
   que para un mismo dongle las claves salen **crecientes** (o que el de deadline menor entra antes).
   Se quita después.
2. **No regresión**: `make` limpio, estrés `n = 1..8` sin cuelgues, `user 0.00 s`, cooldown escalando.

La comparación EDF-vs-FIFO de verdad (mismos parámetros, FIFO se agota y EDF no) es el checkpoint de
**M8**, cuando exista el monitor.

### Preguntas antes de escribir código

1. ¿Por qué la clave de EDF **no** es `get_time_ms() + time_to_burnout` dentro de `take_dongle`?
2. ¿Qué tiene de malo que `last_compile` valga `0` la primera vez?
3. Un coder lleva 200 ms sin compilar y otro acaba de compilar. ¿Dónde queda cada uno en el heap y
   por qué eso es exactamente EDF?
4. ¿Por qué `seq` sigue haciendo falta si ya tenemos `key`?

### Checkpoint

- [x] `make` limpio; `fifo` se comporta **igual** que en M6 (no regresión).
- [x] Con `edf`, las claves del heap son deadlines (`last_compile + time_to_burnout`).
- [x] `last_compile` se actualiza al empezar cada compilación.
- [x] Estrés `n = 1..8` sin cuelgues y sin busy-wait.

> La comparación **EDF salva, FIFO se agota** no se puede medir hasta que exista el monitor (M8):
> hoy nada detecta un burnout, así que la simulación simplemente termina por compilaciones. Ese es
> el primer test de M8.

---

## LECCIÓN 25: M8 — El monitor y la detección de burnout

**Estado: 🔴 EN CURSO.** `monitor.c` escrito (5 funciones), header arreglado y `run_sim` ya crea y
une el monitor. Falta: arreglar la semántica de `mon_start` y que `coder_loop` consulte `stop`.
Ver *“Punto exacto donde lo dejamos”* al principio de la guía.

> Decisión tomada: **opción A** (leer `last_compile` sin lock; el muestreo de 1 ms absorbe el
> desfase). Se documentará en el README, sección *Thread synchronization mechanisms*.

Hasta ahora **nadie mira el reloj**: los coders compilan hasta llegar a `compiles_required` y listo.
Si un coder se queda sin dongles demasiado tiempo, el programa ni se entera. El subject exige un
**hilo monitor separado** que detecte el agotamiento con precisión (≤ 10 ms) y **pare la simulación**.

### Qué tiene que hacer el monitor

```
bucle:
    si todos llegaron a compiles_required:  → stop y salir
    para cada coder con trabajo pendiente:
        deadline = last_compile + time_to_burnout
        si now >= deadline:
            log "now X burned out"
            stop = 1 y salir
    dormir 1 ms
```

Tres decisiones, y el orden importa: **primero "¿todos terminaron?"**, luego los deadlines. Al
revés, el último coder que acaba podría salir declarado `burned out` en el mismo instante en que
termina.

Un solo burnout para la simulación entera: el primero que se agota gana.

### Las cuatro piezas nuevas

| Pieza | Dónde | Para qué |
|---|---|---|
| `thread` del monitor | `t_sim` (ya declarado, sin usar) | `pthread_create` en `run_sim` |
| `stop` | `t_sim` (ya declarado) | bandera de parada, **protegida por `stop_mutex`** |
| `monitor_loop` | `monitor.c` | el `while` de arriba |
| `log` mínimo | `monitor.c` (M9 lo moverá a `log.c`) | `timestamp X burned out` bajo `log_mutex` |

**Pseudocódigo del monitor:**

```text
PSEUDO — all_done(sim)
    para cada coder:
        si compile_times < compiles_required → devolver 0
    devolver 1

PSEUDO — check_burnout(sim)
    ahora ← tiempo actual en ms           (UNA sola vez)
    para cada coder:
        si ya cumplió su cupo → siguiente coder
        si ahora >= last_compile + time_to_burnout:
            escribir "ahora coder_id burned out" con el mutex del log
            devolver 1
    devolver 0

PSEUDO — call_stop(sim)
    bloquear stop_mutex ; stop ← 1 ; desbloquear

PSEUDO — sim_stopped(sim)
    bloquear stop_mutex ; copiar stop ; desbloquear
    devolver la copia

PSEUDO — monitor_loop(arg)
    sim ← arg
    repetir para siempre:
        si all_done(sim)       → call_stop(sim) ; salir
        si check_burnout(sim)  → call_stop(sim) ; salir
        dormir 1 ms
    terminar el hilo
```

### 🪤 La trampa grande: los que duermen en un `cond_wait`

Si el monitor pone `stop = 1` y se va, los coders bloqueados en `pthread_cond_wait(&dongle->cond, ...)`
**no se enteran solos**. ¿Se cuelga entonces el `pthread_join`? **No, si se respeta el invariante:**

> **Todo coder que sale del bucle ha soltado antes sus dongles.**

Con ese invariante, cada salida hace `pthread_cond_broadcast` en `release_dongle` y despierta al
siguiente. La cadena se vacía sola: el que despierta coge los dongles, ve `stop` **después** de
`acquire_dongles`, los suelta sin compilar y sale. Por eso **el monitor no necesita hacer ningún
`broadcast`**: el diseño lo hace innecesario.

Si rompes el invariante (un `break` **antes** de `release_dongles`, o comprobar `stop` dentro del
`while` de `take_dongle` y salir sin sacar tu petición del heap), sí hay cuelgue.

**Coste conocido:** el vaciado tarda como mucho `n × dongle_cooldown` (cada coder espera el
cooldown del anterior). Con cooldown pequeño es instantáneo; con cooldown grande puede notarse.
La solución fina (abandonar la petición en el heap al ver `stop`) es de **M10**.

Y `stop` **no** se lee a pelo: se lee con `stop_mutex` a través de `sim_stopped()`, o no vale de nada.

### 🪤 La segunda trampa: el "≤ 10 ms"

`usleep(10000)` en el monitor **no** cumple: si el deadline vence justo después de despertarse, el
aviso llega 10 ms tarde. Duerme **1 ms** (o usa `pthread_cond_timedwait` con 1 ms). El coste es
despreciable (una comprobación de `n` enteros por ms) y el margen pasa a ser ~1 ms.

Y ojo: el log de burnout se escribe **en el monitor**, no en el coder. Un coder que se agota no
puede avisar de su propia muerte (podría estar esperando un dongle, o el agotamiento ocurre
mientras duerme).

### 🪤 La tercera: lectura de `last_compile`

`last_compile` lo escribe cada coder y lo lee el monitor. En x86-64 un `long` alineado se lee/escribe
de una pieza, así que en la práctica no se ve un valor a medias; pero **la consistencia no está
garantizada** por el estándar. Opciones:

- **A (simple, válida para este proyecto):** leerlo sin lock y aceptar el margen de 1 ms del muestreo.
- **B (estricta):** `signal_mutex` global para publicar `last_compile`.

Decide y **documéntalo en el README** (sección *Thread synchronization mechanisms*). Yo iría a la A
y lo dejaría escrito, porque el subject pide precisión de 10 ms, no atomicidad perfecta.

### Cómo se prueba (y por fin se prueba EDF)

1. **Burnout forzado**: `time_to_burnout` pequeño frente al ciclo (p. ej. `n = 3`,
   `burnout = 50`, `comp = 200`). Debe salir **una** línea `X burned out` y terminar en < 100 ms,
   no quedarse colgado.
2. **Precisión**: medir con `get_time_ms` alrededor; el aviso no puede salir más de 10 ms después
   del deadline real.
3. **EDF vs FIFO**: mismos parámetros viables con `n` impar y mucha contención. Con `fifo` puede
   agotarse alguien; con `edf` **no debe agotarse nadie**. Ese es el checkpoint estrella del
   proyecto.
4. **No regresión**: sin burnout, todo sigue terminando como en M7.

### Preguntas antes de escribir código

1. ¿Por qué el monitor no puede ser un coder más ni vivir dentro de `coder_loop`?
2. Si un coder sale del bucle **sin** soltar sus dongles, ¿dónde se cuelga la simulación? (Pista:
   el `broadcast` de `release_dongle` es lo único que despierta a los que esperan.)
3. ¿Por qué comprobar `stop` **después** de `acquire_dongles` y no dentro del `while` de
   `take_dongle`?
4. ¿Por qué el mensaje de burnout lo escribe el monitor y no el coder agotado?

### Checkpoint

- [x] `monitor.c` escrito y compilando (5 funciones).
- [ ] Existe un hilo monitor creado en `run_sim` y unido con `pthread_join`.
- [ ] Provoco un burnout a propósito y el mensaje sale a tiempo (≤ 10 ms).
- [ ] Todos los hilos paran: `pthread_join` vuelve y no hay cuelgue.
- [ ] Con `edf` y parámetros viables, **nadie se agota** (mientras `fifo` sí puede).
- [ ] valgrind sin leaks ni carreras detectadas por helgrind (opcional).

### 🪤 Errores reales de este hito

| Error | Síntoma | Regla |
|---|---|---|
| `all_done` devolvía 0 al ver **uno** terminado | "todos han acabado" con 0 compilaciones; o nunca acaba | para "¿todos?", devuelve 0 al primero que **no**; 1 solo tras el recorrido |
| `while (1)` + `continue` sin `i++` | bucle infinito en el monitor | `continue` se salta el incremento: invierte la condición del `if` |
| `monitor_loop` devolvía `void` | `-Wreturn-type`; el header y el fichero no coinciden | un hilo devuelve `void *` y acaba en `return (NULL);` |
| marcar `stop` sin `break` | el monitor repite el log y no sale; `join` cuelga | parar = marcar **y** salir |
| `sim->stop = 1;` a pelo | carrera con el lector | solo `call_stop()`, con `stop_mutex` |
| `pthread_mutex_lock(sim->log_mutex)` | no compila | se pasa **la dirección**: `&sim->log_mutex` |
| ayudante `printf` con `t_sim *` y llamado con un coder | tipos cruzados, `coder_id` no existe | si la función imprime un coder, recibe `t_coder *`; o el `printf` va dentro de `check_burnout` |

---

## LECCIÓN 26: M9 — Log serializado

**Estado: ⬜ Pendiente.**

Centraliza todas las escrituras en una función que usa el **mutex de log**.

- [ ] Lanzo `n` grande y **nunca** se mezclan dos mensajes en una línea.

---

## LECCIÓN 27: M10 — Terminación y limpieza (sin leaks)

**Estado: ⬜ Pendiente.**

Terminar cuando: alguien se agota **o** todos compilan ≥ `number_of_compiles_required`.
`join` de todos los hilos y `cleanup` total (mutex, cond, heaps).

- [ ] `valgrind --leak-check=full` no reporta fugas ni en el caso normal ni en el de burnout.

---

# PARTE V: VERIFICACIÓN

## LECCIÓN 28: Norminette y estilo

**Estado: ⬜ Pendiente (se comprueba en cada hito).**

`norminette coders/` hasta **0 errores**. Ojo: también cuentan los archivos/funciones extra.

### Reglas que causaron errores reales en M2 (apúntalas)

- **Espacios alrededor de los operadores**: `sizeof(t_dongle) * n`, nunca `sizeof(t_dongle)*n`.
- **Alineación de prototipos en headers**: todos los nombres de función deben caer en la **misma
  columna**. Como `int` (3 letras) y `void` (4) no miden lo mismo, necesitan distinto número de
  tabs: `int` + **2 tabs**, `void` + **1 tab**.

```c
int		init(t_sim *sim);        /* int  + 2 tabs  */
void	cleanup(t_sim *sim);     /* void + 1 tab   */
```

- **Declaraciones al principio** del bloque: `t_sim sim;` va justo tras `{`, no después de un `if`.
- **5 funciones por archivo, 25 líneas por función**: si `init` se pasa, parte en helpers
  (`init_dongles`, `init_coders`); si el archivo llega a 5, crea uno nuevo (`coder.c`, `log.c`...).
- **Archivos de prueba**: cualquier `.c` suelto en `coders/` cuenta para la Norm. Los experimentos
  (como un `testpthread.c`) van fuera o se borran antes del checkpoint.
- **Las extensiones "42 formatter" de VS Code no son el norminette**: pueden formatearte el código
  con reglas distintas (quitando espacios que la Norm exige). El juez es la CLI: `norminette coders/`.

- [ ] Norminette limpio en todo `coders/`.

---

## LECCIÓN 29: Leaks y sanitizers

**Estado: ⬜ Pendiente (se comprueba en M2 cuando exista `cleanup`).**

- `valgrind --leak-check=full`
- `valgrind --tool=helgrind` (carreras)
- Recompila con `-fsanitize=thread` para cazar *data races*

- [ ] Sin fugas y sin *data races* reportados.

---

## LECCIÓN 30: Pruebas y casos límite

**Estado: ⬜ Pendiente (hay pruebas parciales de M1 ya pasadas).**

Prueba a mano (y guarda tus comandos):

- `number_of_coders = 1` (¡un solo dongle!)
- Parámetros **viables** vs **inviables** para EDF.
- `dongle_cooldown = 0` y valores grandes.
- Muchos coders (10–50) para forzar carreras.
- Entradas inválidas (`-1`, `abc`, `fifoX`, faltan args).

- [ ] Tengo una lista de comandos de prueba reproducibles.

---

## LECCIÓN 31: El README

Incluye (mira `AGENTS.md`): primera línea en cursiva, **Descripción**, **Instrucciones**, **Recursos**
(con uso de IA), y las dos secciones obligatorias: **Blocking cases handled** y
**Thread synchronization mechanisms**.

- [ ] El README está completo y explica *por qué*, no solo *qué*.

---

# PARTE VI: DEFENSA

## LECCIÓN 32: La recode

Durante la evaluación pueden pedirte un **cambio pequeño** (ajustar comportamiento, unas líneas, un dato nuevo).
Prepárate: ten el proyecto compilando, entiende tus estructuras y sé rápido y seguro tocándolas.

- [ ] Sé hacer un cambio pequeño en caliente sin romper la Norm ni los leaks.

---

## LECCIÓN 33: Preguntas típicas de evaluación

Prepárate para explicar **por qué**, no solo **qué**:

1. ¿Cómo evitas el deadlock? ¿Qué condición de Coffman rompes?
2. ¿Cómo garantizas que el log no se mezcla?
3. ¿Qué diferencia hay entre FIFO y EDF y cuándo se agota alguien?
4. ¿Por qué un monitor separado y no comprobar dentro de los coders?
5. ¿Cómo detectas el burnout dentro de los 10 ms?
6. ¿Qué pasa si `dongle_cooldown` es gigante? ¿Y si `number_of_coders` es 1?
7. ¿Dónde está el cooldown y por qué al soltar?

- [ ] Sé responder a todas sin mirar el código.

---

# ANEXO

## Anexo Rápido: tabla de diagnóstico (síntoma → causa)

> Añadida tras M2. Guárdala: cuando algo falle, busca el síntoma y casi seguro está aquí.

| Síntoma | Causa probable | Lección |
|---|---|---|
| `error: expected expression before '...'` | código a medio escribir (el `...` no es C) | 0 |
| `unused parameter` con `-Werror` | la función aún no usa su argumento | — |
| El programa compila pero no hace nada visible | falta una llamada en `main` (código "huérfano") | 1.2 |
| `return` con valor en `void` / viceversa | la firma no cuadra con el header | 14 |
| `passing argument ... from incompatible pointer type` | le pasas un `t_coder *` donde espera un `pthread_mutex_t *` | 14 |
| Valgrind: "definitely lost" | un `malloc` sin `free` (¿dentro de un bucle?) | 6.5.2 |
| Valgrind: "Invalid read/write" | acceso a índice fuera de rango o a memoria ya liberada | 6.5.3, 6.5.7 |
| Segfault al recorrer el bucle de `cleanup` | `= NULL` **dentro** del bucle | 6.5.7 |
| Programa colgado sin mensaje | falta un `unlock`, o deadlock (Coffman), o `join` faltante | 1.3, 4 |
| El hilo nunca imprime nada | `main` hizo `return` sin `join` (el SO mata los hilos) | 1.3 |
| Primer elemento del array con basura | bucle con `i = 1` en vez de `i = 0` | 6.5.3 |
| ids desplazados en el log (empiezan en 2) | `i = 1` por querer que el id empiece en 1 | 6.5.4 |
| `if (comprobar_algo)` no detecta errores nunca | falta los paréntesis: `if (comprobar_algo())` | 6.5.6 |
| `too few arguments to function call, expected 2, have 0` | a un `mutex_init`/`cond_init` le faltan **qué** y los atributos | 21 |
| `expected identifier` en `x->` | el nombre no existe en ese struct (te has dejado un trozo) | 21, 14 |
| Compila, pero revienta cuando un `pthread_create` falla | doble `cleanup`: la segunda vez sobre `NULL` | 6.5.7 |
| Un `x == NULL` "no asigna nada" | `==` compara; `=` asigna | 6.5.6 |
| `n = 1` funciona pero `n = 4` revienta (o al revés) | caso límite no trazado a mano | 6.5 |
| Un solo coder avanza y el resto espera | mutex global en vez de uno por recurso | 2.2 |
| Dos mensajes del log en la misma línea | falta el mutex de log | 9 |

**Regla de oro del debugging en concurrencia:** traza el bucle **a mano** con `n = 1`, `n = 2` y
`n = 4` antes de ejecutar. Casi todos los bugs de M2 se veían en la primera vuelta del papel.
Y recuerda: la concurrencia castiga 1 de cada 10.000 ejecuciones — si "a veces falla", es una
carrera, no un error de sintaxis.

## Glosario

| Término | Significado |
|---|---|
| **Hilo (thread)** | Persona dentro del proceso; comparte memoria, tiene pila propia |
| **Mutex** | Candado: solo un hilo dentro de la sección crítica |
| **Condición de carrera** | Fallo por acceso concurrente no sincronizado a datos compartidos |
| **Variable de condición** | Permite dormir hasta que alguien avise |
| **Deadlock** | Cuelgue: cada hilo espera a otro para siempre |
| **Coffman** | Las 4 condiciones que deben darse para un deadlock |
| **Starvation** | Un hilo nunca consigue el recurso |
| **Scheduler** | Regla de arbitraje (fifo / edf) |
| **FIFO** | First In First Out: orden de llegada |
| **EDF** | Earliest Deadline First: el deadline más próximo primero |
| **Deadline** | `last_compile_start + time_to_burnout` |
| **Burnout** | Agotamiento: no empezó a compilar a tiempo |
| **Dongle** | Recurso escaso (uno por persona); hacen falta 2 para compilar |
| **Cooldown** | Tiempo que un dongle queda inutilizable tras soltarse |
| **Heap / cola de prioridad** | Estructura que da siempre el mínimo (más urgente) |
| **Monitor** | Hilo que vigila deadlines y detiene la simulación |
| **Sección crítica** | Trozo de código que solo un hilo puede ejecutar a la vez |
| **Serializar (log)** | Escribir mensajes enteros, sin mezclarse |

## Checklist final

- [ ] Compila con `cc -Wall -Wextra -Werror -pthread`, sin relink.
- [ ] `norminette` = 0 errores en **todos** los archivos.
- [ ] Sin variables globales. Sin libft. Solo funciones autorizadas.
- [ ] Sin leaks (`valgrind`) y sin *data races* (`helgrind`/tsan).
- [ ] Rechaza entradas inválidas.
- [ ] Cooldown correcto; planificador FIFO y EDF correctos; EDF no deja morir a nadie.
- [ ] Monitor detecta burnout ≤ 10 ms y para la simulación.
- [ ] Log serializado con el formato exacto.
- [ ] Termina por burnout **o** por compilaciones completadas.
- [ ] README con todas las secciones obligatorias.
- [ ] Sé explicar y modificar cualquier parte (para la defensa).

## Mapa del subject → dónde se cumple

| Regla del subject | Dónde se cumple (archivo/función) |
|---|---|
| Hilos POSIX | `coder.c`, `monitor.c` |
| Mutex por dongle | `dongle.c` |
| Variable de condición | `dongle.c` (espera de dongles/cooldown) |
| Cooldown | `dongle.c` |
| Arbitraje FIFO/EDF | `heap.c` + `dongle.c` |
| Cola de prioridad (heap) | `heap.c` |
| Monitor + burnout ≤ 10 ms | `monitor.c` |
| Log serializado | `log.c` |
| Sin leaks | `cleanup.c` + `valgrind` |
| Sin globales / funciones autorizadas | todo el proyecto (`AGENTS.md`) |

> **De qué ficheros habla esta tabla y cuáles existen ya:**
>
> | Fichero | ¿Existe? | Nota |
> |---|---|---|
> | `main.c`, `parser.c`, `init.c`, `sim.c`, `log.c` | ✅ | M1–M3 |
> | `coder.c` | ✅ | ahora mismo contiene también `take_dongle()` / `release_dongle()` |
> | `dongle.c` | ❌ | lo creas en M4 (puedes dejarlo en `coder.c` si la Norm te obliga a partir) |
> | `heap.c` | ✅ hecho en M6 | compare `key` (FIFO=seq, EDF=deadline) |
> | `monitor.c` | ❌ | lo creas en M8 (`cleanup()` sigue en `init.c`) |
>
> O sea: la tabla es el **destino**, no el estado actual. Cuando crees o muevas un
> fichero, actualízala (y la tabla de archivos sugeridos de la Lección 14.2).

## Recursos para estudiar

- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`, `man gettimeofday`.
- **The Little Book of Semaphores** (Allen Downey) — clásico de sincronización.
- Wikipedia: *Dining philosophers problem*, *Coffman conditions*, *Earliest deadline first*.
- Norm de 42: https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf
- El propio subject (`es.subject.pdf`) — léelo antes de cada parte.

---

**Fin de la guía. ¡Mucha suerte con Codexion! Cuando quieras, empezamos por la Lección 1.**

# Skills del proyecto — procedencia y licencias

Estas skills se han **copiado tal cual** (sin inventar ni reescribir su contenido) desde
repositorios públicos, y se han instalado en `.opencode/skills/` para que OpenCode las
descubra en este proyecto.

El objetivo es **aprender**: acompañar a quien programa durante el proyecto *Codexion*,
guiándole para que entienda y escriba él mismo el código, en lugar de generárselo —
manteniendo siempre las **reglas del subject** (ver `../AGENTS.md`).

## Skills instaladas

### Enseñanza / aprendizaje

| ID (carpeta) | Fuente (copia literal) | Licencia | Para qué sirve aquí |
|---|---|---|---|
| `tutor-c` | **Skill propia del proyecto** — creada el 2026-10-20 a partir de lo observado en las sesiones reales de M0–M2 (errores repetidos, estilo de dudas, qué explicaciones funcionaron) | propia | Calibrada al alumno real: perfil junior en C, orden de explicación (qué es → por qué → analogía → trampa → su turno), volumen corto, formato de revisión "síntoma → traza con `n=1`/`n=4` → causa → regla", lista de sus errores típicos. **Tiene prioridad** sobre las genéricas cuando varias encajen. |
| `teaching-style` | Skill local ya existente en `~/.config/opencode/skills/teaching-style/SKILL.md` | propia del usuario | Estilo de enseñanza en español: paso a paso, preguntas guía, no dar el código, explicar el porqué. |
| `socratic-tutor` | https://gist.github.com/Shunseii/8cd4ef40718a2f2966ca8f30fa17cad6 | gist público, sin licencia explícita | Modo tutor para escribir código: pistas en vez de respuestas, revisa tu código sin reescribirlo. |
| `socrates` | https://github.com/bevibing/socrates-skill (`SKILL.md`) | MIT © 2026 RoundTable02 | Método socrático puro: nunca da la respuesta, te guía con preguntas. |
| `teach` | https://github.com/mattpocock/skills → `skills/productivity/teach/` | MIT © 2026 Matt Pocock | Espacio de enseñanza persistente: misión, lecciones, glosario y registros de aprendizaje entre sesiones. |

### Código limpio / senior / reducido / diseño

| ID (carpeta) | Fuente (copia literal) | Licencia | Para qué sirve aquí |
|---|---|---|---|
| `martin-clean-code` | https://github.com/nghorbani/clean-code-skill (`SKILL.md`, `cheatsheet.md`, `glossary.md`, `patterns.md`) | CC BY 4.0 | Referencia de código limpio (Clean Code): nombres, funciones, manejo de errores, olores/heurísticas G1–G36, concurrencia. |
| `minimalist` | https://github.com/alirezarezvani/claude-skills → `engineering/minimalist/SKILL.md` | MIT © 2025 Alireza Rezvani | Código **reducido**: evita sobre-ingeniería, dependencias y abstracciones innecesarias (escalera YAGNI → reutilizar → stdlib → …). |
| `code-review` | https://github.com/mattpocock/skills → `skills/engineering/code-review/` | MIT © 2026 Matt Pocock | Revisión **senior** en dos ejes: estándares del repo vs. spec original (lanza subagentes en paralelo). |
| `codebase-design` | https://github.com/mattpocock/skills → `skills/engineering/codebase-design/` | MIT © 2026 Matt Pocock | Diseño **sofisticado**: vocabulario de *módulos profundos*, dónde poner la costura/seam, oportunidades de profundización. |

### Diseño (instalada antes)

| ID (carpeta) | Fuente (copia literal) | Licencia | Para qué sirve aquí |
|---|---|---|---|
| `software-design-philosophy` | https://github.com/luoling8192/software-design-philosophy-skill (`SKILL.md`) | MIT | Diseño de software (Ousterhout): módulos profundos, ocultar información, nombrar bien, reducir complejidad. |

### Ficheros de apoyo

`SKILL.md` referencia ficheros adicionales, copiados también literalmente en la misma carpeta:

- `teach/`: `MISSION-FORMAT.md`, `RESOURCES-FORMAT.md`, `GLOSSARY-FORMAT.md`, `LEARNING-RECORD-FORMAT.md`.
- `codebase-design/`: `DESIGN-IT-TWICE.md`, `DEEPENING.md`.
- `martin-clean-code/`: `cheatsheet.md`, `glossary.md`, `patterns.md`.

## Notas de fidelidad / portabilidad

- **`teach`**: conserva el frontmatter original `disable-model-invocation: true`.
  Si se quisiera que el agente lo active solo, bastaría con quitar esa línea.
- **`martin-clean-code`**: conserva `allowed-tools` / `argument-hint` (metadatos de Claude).
  Son inofensivos en OpenCode.
- **`socratic-tutor`**: los ejemplos del texto original son de TypeScript/React; son ilustrativos,
  no instrucciones para este proyecto en C.
- **Sin skills inventadas de fuentes externas**: no existe skill pública descargable específica de Norminette/42 ni de
  pthreads en C, así que **no se ha fabricado ninguna de fuera**. Las reglas del subject se han fijado en
  `AGENTS.md` (mecanismo oficial de instrucciones de OpenCode V2), no como skill.
  **Excepción declarada: `tutor-c`**, que es una skill **propia** (no copiada de ningún repo),
  escrita en este proyecto a partir del patrón de enseñanza observado en las sesiones reales.
- **Prioridad entre skills de enseñanza**: `tutor-c` > `teaching-style` ≈ `socratic-tutor` > `socrates`.
  Si se cargan varias, `tutor-c` manda por estar calibrada a este alumno y este repo; las otras
  quedan como referencia de método (sócrates puro, espacio de lecciones de `teach`).

## Cómo se usan en OpenCode

- El agente las descubre por su `description` y las carga cuando encajan con la petición.
- También se pueden forzar manualmente: `@socrates`, `@socratic-tutor`, `@teaching-style`,
  `@martin-clean-code`, `@minimalist`, `@code-review`, `@codebase-design`,
  `@software-design-philosophy`, `@teach`.
- Las **reglas del subject** viven en `AGENTS.md` (raíz del repo) y se cargan siempre.

## Ubicación

```
AGENTS.md                     ← reglas obligatorias del subject (siempre activas)
.opencode/
├── SOURCES.md                ← este archivo
└── skills/
    ├── code-review/SKILL.md
    ├── codebase-design/{SKILL.md,DESIGN-IT-TWICE.md,DEEPENING.md}
    ├── martin-clean-code/{SKILL.md,cheatsheet.md,glossary.md,patterns.md}
    ├── minimalist/SKILL.md
    ├── socrates/SKILL.md
    ├── socratic-tutor/SKILL.md
    ├── software-design-philosophy/SKILL.md
    ├── teach/{SKILL.md,*-FORMAT.md}
    └── teaching-style/SKILL.md
```

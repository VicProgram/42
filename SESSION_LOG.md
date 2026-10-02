# Sesión de Optimización y Reorganización

**Fecha:** 2026-10-02
**Usuario:** VicProgram (Victor Abad Romero)

---

## 1. Reorganización de GitHub

### Repositorios de 42 consolidados
- Se creó el repo `42` (https://github.com/VicProgram/42) con todos los proyectos de la escuela
- Repos incluidos: Libft, Get-next-line, Printf, Push_Swap, C-library, RLE-File-Compressor-, A_maze_ing, Exam03, fly_ing, Call_Me_Maybe, Piscine-Discovery-Ciberseguridad
- Los repos individuales de 42 siguen en el perfil (no eliminados)

### Repos personales (sin cambios)
- modelling-simulation-lab
- MotorSport
- WebcamControl
- Python-Exercises
- Number-Guessing-Name

---

## 2. Correcciones de código

### Libft
| Archivo | Problema | Corrección |
|---|---|---|
| `ft_putnbr_fd.c` | No manejaba `INT_MIN` (overflow con `-n`) | Usar `long` en lugar de `int` |

### Push_Swap
| Archivo | Problema | Corrección |
|---|---|---|
| `rr_move.c` | `rrr_move` llamaba `ra_move`/`rb_move` en lugar de `rra_move`/`rrb_move` | Corregido |

### Exam03
| Archivo | Problema | Corrección |
|---|---|---|
| `CONAITOR/bracket_validator.py` | Incompleto | Implementado con stack |
| `Medium/py_bracket_validator.py` | Lógica rota | Reemplazada con stack válido |
| `Hard/py_string_permutation_checker.py` | No verificaba longitud | Añadida verificación |

### Exam03 consolidado
- Todos los ejercicios se consolidaron en `Exam03/exam03.py`
- Se eliminaron los archivos duplicados (CONAITOR, Easy, Medium, Hard, SOLO)

---

## 3. Archivos binarios eliminados
- Se eliminaron todos los archivos `.o` y ejecutables
- Se creó `.gitignore` con `*.o` y `push_swap`

---

## 4. PDF a Markdown

### Herramienta instalada
- **pdf-to-markdown** (Nutrient) - instalada globalmente con npm
- Comando: `pdf-to-markdown input.pdf output.md`

### Herramienta desinstalada
- **MarkItDown** - desinstalada (entorno virtual eliminado)

---

## 5. Windows 11 Pro - Tweaks aplicados

### Servicios desactivados
| Servicio | Descripción |
|---|---|
| `DiagTrack` | Telemetría |
| `MapsBroker` | Mapas |
| `WSLService` | WSL |
| `WSAIFabricSvc` | IA de Windows |
| `Bonjour Service` | Bonjour |
| `Fax` | Fax |
| `RetailDemo` | Retail Demo |
| `wisvc` | Windows Insider |
| `Spooler` | Impresión |
| `Themes` | Temas |
| `TextInputManagementService` | Text Input |
| `edgeupdate` | Edge Update |

### Servicios NO desactivados (por precaución)
| Servicio | Motivo |
|---|---|
| `UAHelperService` | Universal Audio - se usa con plugins |
| `WSearch` | Búsqueda de Windows - puede ser útil |
| `SysMain` | Superfetch - puede ser útil en HDD |

### Tweaks recomendados pero NO aplicados
- Plano de energía Ultimate Performance
- Efectivos visuales ajustados
- Desactivar resultados web en Inicio (registry)
- Desactivar telemetría opcional
- Integridad de memoria (Core Isolation)
- Acceso controlado a carpetas

---

## 6. Revertir cambios

### GitHub
```bash
# Ver historial de commits
git log --oneline

# Revertir un commit específico
git revert <commit-hash>

# Volver a un commit anterior
git reset --hard <commit-hash>
```

### Windows - Revertir servicios
```powershell
# Revertir un servicio
Set-Service -Name "NombreDelServicio" -StartupType Automatic
Start-Service -Name "NombreDelServicio"

# Ejemplo: revertir DiagTrack
Set-Service -Name "DiagTrack" -StartupType Automatic
Start-Service -Name "DiagTrack"
```

### Windows - Punto de restauración
- Busca "Crear un punto de restauración" en Inicio
- Selecciona un punto de restauración anterior a los cambios
- Click en "Restaurar sistema"

---

## 7. Notas importantes

- Los repos individuales de 42 siguen en GitHub (no se eliminaron)
- El repo `42` es un "espejo" de los proyectos de la escuela
- Los cambios en Windows son reversibles
- Se recomienda crear un punto de restauración antes de más tweaks

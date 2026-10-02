# rle — Compresión para archivos binarios

Un compresor y descompresor RLE mínimo y seguro para binarios escrito en C. Funciona correctamente con cualquier formato de archivo: texto plano, imágenes PPM, pixel art, BMP y cualquier otro flujo de bytes.

---

## Qué es RLE

La Codificación por Longitud de Ejecución es un algoritmo de compresión sin pérdida que reemplaza bytes consecutivos repetidos con un único par (byte, cantidad). Es más efectivo en datos con largas secuencias homogéneas: pixel art, imágenes simples, gráficos generados o formatos binarios estructurados con relleno.

Ejemplo:

```
Entrada:  AAABBBBBCC
Salida: A3 B5 C2
```

Para imágenes fotográficas o datos de alta complejidad, RLE ofrece poca o ninguna ganancia de compresión.

---

## Formato de archivo

Los archivos comprimidos producidos siguen una estructura binaria simple:

```
[ 4 bytes: cabecera mágica "RLE\x01" ]
[ N pares: 1 byte valor | 1 byte cantidad ]
```

La cabecera mágica permite al descompresor rechazar archivos que no fueron producidos por esta herramienta y detectar usos incorrectos accidentales.

Las cantidades van de 1 a 255. Una secuencia mayor de 255 bytes se divide automáticamente en múltiples pares.

---

## Compilación

Sin dependencias más allá de un compilador de C.

```bash
gcc -o rle rle.c
```

O con estándar explícito:

```bash
gcc -std=c99 -Wall -Wextra -o rle rle.c
```

---

## Uso

```
rle <compress|decompress> <archivo_entrada> <archivo_salida>
```

### Comprimir un archivo

```bash
./rle compress image.ppm image.ppm.rle
```

### Descomprimir un archivo

```bash
./rle decompress image.ppm.rle image_restored.ppm
```

### Verificar integridad de ida y vuelta

```bash
./rle compress original.ppm compressed.rle
./rle decompress compressed.rle restored.ppm
diff original.ppm restored.ppm && echo "OK"
```

---

## Formatos soportados

Cualquier formato binario funciona como entrada. Algunos ejemplos donde RLE funciona bien:

| Formato           | Notas                                                           |
| ----------------- | --------------------------------------------------------------- |
| PPM / PGM / PBM   | Formatos de píxeles sin compresión con regiones de color sólido |
| BMP               | Datos de mapa de bits sin compresión                            |
| Pixel art crudo   | Datos de color indexado con grandes áreas planas                |
| Archivos de texto | Efectivo solo si hay repeticiones de caracteres                 |
| Cualquier binario | Funciona correctamente independientemente del contenido         |

Formatos donde RLE ofrece poca o ninguna mejora de compresión:

| Formato    | Motivo                                     |
| ---------- | ------------------------------------------ |
| JPEG       | Ya está comprimido; alta entropía          |
| PNG        | Ya utiliza compresión DEFLATE internamente |
| MP3 / OGG  | Audio comprimido                           |
| ZIP / gzip | Ya comprimidos                             |

---

## Decisiones de diseño

**Entrada/Salida en modo binario.** Los archivos se abren con `"rb"` y `"wb"` para evitar cualquier traducción de finales de línea en Windows. Esto es necesario para la corrección con formatos binarios.

**Archivos nombrados en lugar de stdin/stdout.** Pasar datos binarios a través de un shell puede corromperlos dependiendo de la plataforma y la configuración del terminal. Los argumentos de archivo explícitos evitan esto por completo.

**Cabecera mágica.** La cabecera de cuatro bytes `RLE\x01` permite al descompresor rechazar entradas inválidas inmediatamente en lugar de producir salida basura silenciosamente.

**Codificación uniforme por pares.** Cada secuencia, incluidas las de longitud 1, se almacena como un par (byte, cantidad). Esto simplifica el decodificador a un bucle compacto sin casos especiales.

**Sin asignación dinámica de memoria.** El compresor y descompresor operan en memoria O(1) usando solo dos variables enteras para el estado.

---

## Manejo de errores

La herramienta sale con un estado distinto de cero e imprime un mensaje en stderr en los siguientes casos:

* No se puede abrir el archivo de entrada o salida
* No se puede escribir la cabecera en el archivo de salida
* La cabecera mágica falta o es incorrecta durante la descompresión
* El flujo comprimido termina con un par incompleto (archivo truncado)
* Ocurre un error de escritura durante la descompresión

---

## Limitaciones

* La longitud máxima de una secuencia es de 255 bytes por par. Las secuencias más largas se dividen automáticamente sin pérdida.
* No hay soporte para streaming desde stdin/stdout por diseño (ver arriba).
* No hay informe de ratio de compresión ni modo verboso.
* No es adecuado como compresor de propósito general para datos arbitrarios. Para eso, usa gzip, zstd o lz4.

---

## Licencia

Dominio público. Haz lo que quieras con ello.

Ayuda de IA para recopilar información.
Ayuda de IA para crear la documentación.

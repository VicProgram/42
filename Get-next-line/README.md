*Este proyecto ha sido creado como parte del currículo de 42 por vabad-ro.*

## Descripción

El objetivo de este proyecto es replicar el funcionamiento de la función get_next_line, que permite leer una línea de un archivo o texto introducido por terminal, devolviendo la primera línea hasta el salto de línea (\n). La función debe devolver NULL cuando se haya llegado al final del archivo (EOF) y se vuelva a llamar.

El programa se debe utilizar de manera que abra un archivo o reciba texto por terminal, y cada vez que se invoque la función get_next_line, devuelva una línea del texto hasta el salto de línea. Si se ha llegado al final del texto, debe retornar NULL.

## Instrucciones

Para utilizar get_next_line(fd):

1. **Abrir un archivo o introducir texto por terminal**. 
   - Si se abre un archivo, se debe pasar el descriptor de archivo (fd) a la función.
   - Si se desea introducir texto por terminal, se utilizará el descriptor de entrada estándar.

2. **Hacer la llamada a la función** con el descriptor de archivo (fd) que se ha generado en la apertura del archivo o el de la entrada por terminal.

3. **Recorrer el texto** mediante un bucle, asegurándote de liberar la memoria de la variable estática que se deja declarada al principio.

4. **Al llegar al final del archivo**, se debe invocar la función pasando un descriptor de archivo no válido (por ejemplo, -1996), para que el programa termine correctamente.

### Ejemplo de compilación

Para compilar el programa, utiliza el siguiente comando (ajustado a un tamaño de buffer de 42):

bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 <nombre_archivos>.c

Este proyecto utiliza las siguientes funciones:

- char *get_next_line(int fd); : Lee una línea del archivo o entrada.
- int ft_strlen(const char *s); : Devuelve la longitud de una cadena.
- char *ft_strjoin(char *s1, char *s2);: Une dos cadenas de texto.
- char *ft_strchr(const char *s, int c); : Busca un carácter en una cadena.
- char *ft_makeline(char *stash);: Extrae una línea completa desde el buffer.
- char *ft_makestash(char *stash);: Actualiza el estado del buffer.
- char *ft_clean_memo(char *stash, char *buffer);: Limpia la memoria reservada.

## Algoritmo

El algoritmo funciona de la siguiente manera:

1. **Lectura del archivo o entrada**: El programa comienza leyendo el archivo o texto introducido.
2. **Comprobación de salto de línea**: Si encuentra un salto de línea (\n), extrae la línea completa y la guarda en una variable.
3. **Guardado del punto de lectura**: El programa almacena el punto donde se quedó (después del salto de línea) en una variable estática para poder continuar desde allí en la siguiente llamada.
4. **Retorno de la línea**: La función devuelve la línea leída.
5. **Finalización**: Cuando se alcanza el final del archivo (EOF) y no hay más datos por leer, la función devuelve NULL.

## Recursos

La mayor parte de la información se ha extraído de los siguientes recursos:

- *Peer to Peer*
- *Manuales de Linux*
- [Documentación de IBM](https://www.ibm.com/docs)

## Notas Importantes

- Es necesario liberar correctamente la memoria utilizada por las variables estáticas para evitar fugas de memoria.
- Se debe tener especial cuidado con el manejo del buffer y la actualización del estado entre las llamadas a la función.
- En caso de que no se recorra todo el texto hasta el final, asegúrate de llamar a get_next_line() con un descriptor de archivo no válido (por ejemplo, -1996)
para liberar correctamente los recursos.

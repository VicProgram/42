# *Este proyecto ha sido creado como parte del currículo de 42 por vabad-ro.*


# Descripción
Este proyecto se basa en la creación de una librería propia para el lenguaje C.
Una librería la cual va a poder ser utilizada a lo largo de todo nuestro curso y en
cualquier ocasión que pueda ser de provecho.

Esta librería cubre las necesidades básicas de la programación en *C* sobre todo con el manejo de cadenas (*Strings*), Listas Enlazadas (*Linked List*), manejo de memoria, funciones básicas, etc..

Muestra réplicas de las funciones de los manuales de Linux (*man nombredefuncion*) en su composición, retornos, funcionamiento...

Viene provista de un Makefile preparado para la compilación, junto a un archivo *.h para el correcto funcionamiento, y todos los archivos *.c que van a contener las funciones.


# Instrucciones

Para la utilización  de la librería, debemos tener en el directorio la carpeta contenedora de todos los documentos *( *.c, *.h y makefile)*.

En este caso la hemos nombrado como **"Libft"**.

Existen varias funciones básicas para la gestión de la librería.

	make  -> compila, crea archivos objeto, prepara la librería para funcionar.
		Deja los archivos de las funciones ( *.c) sin modificar.

	make clean  -> hace la limpieza de los archivos objeto.
		Deja los archivos de las funciones ( *.c) sin modificar.

	make fclean  ->  hace una limpieza completa de los archivos resultantes de la compilación.
		Deja los archivos de las funciones ( *.c) sin modificar.

	make -re  -> ejecuta de nuevo la limpieza (fclean) y recompila (make).

A partir de este punto ya estamos en condiciones de poder provechar la librería.
# Listado de funciones

| Función | Descripción |
|--------|-------------|
| ft_atoi | Convierte una cadena de caracteres en un entero (`int`). |
| ft_itoa | Convierte un entero (`int`) en una cadena de caracteres. |
| ft_bzero | Rellena un bloque de memoria con ceros. |
| ft_calloc | Reserva memoria y la inicializa a cero. |
| ft_isalnum | Comprueba si un carácter es alfanumérico. |
| ft_isalpha | Comprueba si un carácter es una letra. |
| ft_isascii | Comprueba si un carácter pertenece a la tabla ASCII. |
| ft_isdigit | Comprueba si un carácter es un dígito numérico. |
| ft_isprint | Comprueba si un carácter es imprimible. |
| ft_strlen | Devuelve la longitud de una cadena. |
| ft_tolower | Convierte una letra mayúscula a minúscula. |
| ft_toupper | Convierte una letra minúscula a mayúscula. |
| ft_memset | Rellena un bloque de memoria con un valor específico. |
| ft_memcpy | Copia un bloque de memoria a otro (sin solapamiento). |
| ft_memmove | Copia un bloque de memoria permitiendo solapamiento. |
| ft_memchr | Busca un byte específico en un bloque de memoria. |
| ft_memcmp | Compara dos bloques de memoria. |
| ft_strdup | Duplica una cadena reservando nueva memoria. |
| ft_strchr | Busca la primera aparición de un carácter en una cadena. |
| ft_strrchr | Busca la última aparición de un carácter en una cadena. |
| ft_strncmp | Compara dos cadenas hasta un número determinado de caracteres. |
| ft_strlcpy | Copia una cadena asegurando la terminación nula. |
| ft_strlcat | Concatena cadenas asegurando la terminación nula. |
| ft_strnstr | Busca una subcadena dentro de un número limitado de caracteres. |
| ft_substr | Devuelve una subcadena a partir de una cadena original. |
| ft_strjoin | Concatena dos cadenas en una nueva. |
| ft_strtrim | Elimina caracteres específicos del inicio y final de una cadena. |
| ft_split | Divide una cadena en un array de strings usando un delimitador. |
| ft_strmapi | Aplica una función a cada carácter de una cadena y crea una nueva. |
| ft_striteri | Aplica una función a cada carácter de una cadena (modificando la original). |
| ft_putchar_fd | Escribe un carácter en un descriptor de archivo. |
| ft_putstr_fd | Escribe una cadena en un descriptor de archivo. |
| ft_putendl_fd | Escribe una cadena seguida de un salto de línea en un descriptor. |
| ft_putnbr_fd | Escribe un número entero en un descriptor de archivo. |
| ft_lstnew | Crea un nuevo nodo de lista enlazada. |
| ft_lstadd_front | Añade un nodo al inicio de una lista enlazada. |
| ft_lstadd_back | Añade un nodo al final de una lista enlazada. |
| ft_lstsize | Devuelve el número de nodos en una lista enlazada. |
| ft_lstlast | Devuelve el último nodo de una lista enlazada. |
| ft_lstdelone | Elimina un nodo usando una función de borrado. |
| ft_lstclear | Elimina y libera todos los nodos de una lista. |
| ft_lstiter | Aplica una función a cada nodo de una lista. |
| ft_lstmap | Crea una nueva lista aplicando una función a cada nodo. |

# Recursos

La mayor parte de información la hemos extraído de:

-	*Peer to Peer*.
-	*Manuales de Linux*
-	*https://www.ibm.com/docs*
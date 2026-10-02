 *Este proyecto ha sido creado como parte del currículo de 42 por vabad-ro.*


# Descripción
Este proyecto se basa en la replicación del funcionamiento de la función "printf" original de C. Se basa en replicar las funciones básicas y alguna avanzada para su posterior uso en el resto de proyectos de el curso.

 **Nos permite imprimir por terminal de forma formateada.**

Tenemos en cuenta los conversores *(nos indican el tipo de formato para la variable a imprimir)*.
Nos permite también saber la cantidad de caractéres impresos ya que la función retorna el número total de carácteres impresos.

Los conversores o *"Place holders"* van precedidos del símbolo '%', tenemos varios para diferentes formatos.

# Instrucciones
De forma sencilla hacemos una llamada a la función ft_printf();

Como primer parámetro dentro tenemos la cadena de carácteres que va a incluir los carácteres '%' y detrás los conversores/place holders para dar formato. 

Como segundo parámetro la variablea imprimir.

	Ejemplo
		ft_printf("%c", 'h');
	Este ejemplo imprimirá en la terminal el carácter 'h'.

		ft_printf("%X", 141);
	Este ejemlo imprime en base hexadecimal con las letras mayúsculas el valor 141.

# Listado de funciones

Las conversiones que tenemos disponibles son :

	-c -> Para carácteres individuales.
	-s -> Para cadenas de carácteres.
	-p -> Para punteros.
	-d -> Para números decimales.
	-i -> Para números enteros.
	-u -> Para números decimales sin signo *(Unsigned)*.
	-x -> Para números hexadecimales con la base con letras minúsculas.
	-X -> Para números hexadecimales con la base con letras mayúsculas.
	-% -> Para imprimir el carácter '%'.

# Algoritmo

Para esta función, la elección de los algoritmos ha sido  basada en la reutilización del código realizado en Libft, otro proyecto de este currículo, pero con ciertas adaptaciones.

Principalmente se ha optado por la forma más sencilla que se pudiera encontrar de replicar la función *"Printf"*.

Para los caracteres seha adaptado ft_putchar, para los strings ft_putstr_fd, para números  ft_printnbr, ft_printhexa (adaptación de ft_putnbr_base), y para las  direcciones de memoria también.

El principal motivo de las readaptaciones de las funciones es la necesidad de convertir las funciones de sus tipos originales a funciones de tipo Int, para así poder contar los carácteres impresos en terminal ya que es el principal retorno de la función original.

Todo esto está bajo el mando de un selector de *"place holders"* el cual determina y limita el uso de estos :	*static int	valid(char c);*

Para el  control del flujo he realizado la comprobación de la cadena de entrada en el parámetro de la función, manejando incluso valores NULL.

Se comprueba que el place holder es válido  y continúa el flujo de la función.

Todo resulta en una impresión por terminal de las cadenas y las variables formateadas y  como return el número de carácteres impresos. Includio  el -1 como error.

# Recursos

La mayor parte de información la hemos extraído de:

-	*Peer to Peer*.
-	*Manuales de Linux*
-	*https://www.ibm.com/docs*
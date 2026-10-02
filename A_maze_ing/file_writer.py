from cell import Cell
from config_data import Map


def cell_to_hex(cell: Cell) -> str:
    """
    Convierte una celda del laberinto a su representación hexadecimal.

    Cada pared de la celda se codifica en un bit:
    - Norte (n) -> bit 0
    - Este (e)  -> bit 1
    - Sur (s)   -> bit 2
    - Oeste (w) -> bit 3

    El valor binario resultante se transforma en un carácter hexadecimal
    para facilitar el almacenamiento compacto del laberinto.

    Args:
        cell (Cell):
            Objeto `Cell` que contiene el estado de las paredes.

    Returns:
        str:
            Carácter hexadecimal que representa la configuración
            de paredes de la celda.
    """
    value = (
        (cell.n & 1)
        | ((cell.e & 1) << 1)
        | ((cell.s & 1) << 2)
        | ((cell.w & 1) << 3)
    )
    return format(value, 'X')


def write_maze_to_file(
    grid: list[list[Cell]],
    game_map: Map,
    route: list[str]
) -> None:
    """
    Guarda un laberinto y su solución en un archivo de texto.

    El archivo generado contiene:
    1. La representación hexadecimal de cada celda del laberinto.
    2. Las coordenadas de entrada.
    3. Las coordenadas de salida.
    4. La ruta de solución como secuencia de movimientos.

    Formato del archivo:
        - Una línea por fila del laberinto.
        - Coordenadas en formato `x,y`.
        - Ruta final como cadena continua de caracteres.

    Args:
        grid (list[list[Cell]]):
            Matriz bidimensional de objetos `Cell`
            que representa el laberinto.

        game_map (Map):
            Configuración del mapa y ruta del archivo de salida.
            Debe contener:
            - output_file
            - entry
            - exit_

        route (list[str]):
            Lista de movimientos de la solución del laberinto.

    Returns:
        None:
            Esta función no devuelve ningún valor.
    """
    with open(game_map.output_file, 'w') as f:
        for row in grid:
            f.write(''.join(cell_to_hex(cell) for cell in row) + '\n')

        f.write(f"\n{game_map.entry[0]},{game_map.entry[1]}\n")
        f.write(f"{game_map.exit_[0]},{game_map.exit_[1]}\n")
        for letter in route:
            f.write(f"{letter}")
        f.write("\n")

from cell import Cell


PATTERN = [
    [1, 0, 0, 0, 1, 1, 1],
    [1, 0, 1, 0, 0, 0, 1],
    [1, 1, 1, 0, 1, 1, 1],
    [0, 0, 1, 0, 1, 0, 0],
    [0, 0, 1, 0, 1, 1, 1],
]
HEIGHT = len(PATTERN)
WIDTH = len(PATTERN[0])


def is_42_cell(x: int, y: int, maze_width: int, maze_height: int) -> bool:
    """
    Determina si una coordenada pertenece al patrón especial "42".

    El patrón se coloca centrado dentro del laberinto siempre que las
    dimensiones sean suficientemente grandes. La función calcula la
    posición relativa de la celda respecto al patrón y comprueba si
    dicha posición está marcada con un `1`.

    Args:
        x (int):
            Coordenada horizontal de la celda.

        y (int):
            Coordenada vertical de la celda.

        maze_width (int):
            Anchura total del laberinto.

        maze_height (int):
            Altura total del laberinto.

    Returns:
        bool:
            `True` si la celda forma parte del patrón "42",
            `False` en caso contrario.
    """
    if maze_width < WIDTH + 2 or maze_height < HEIGHT + 2:
        return False
    start_x = (maze_width - WIDTH) // 2
    start_y = (maze_height - HEIGHT) // 2
    px, py = x - start_x, y - start_y
    if 0 <= py < HEIGHT and 0 <= px < WIDTH:
        return PATTERN[py][px] == 1
    return False


def apply_to_matrix(
        matrix: list[list[Cell]], maze_width: int, maze_height: int
        ) -> list[list[Cell]]:
    """
    Aplica el patrón especial "42" sobre una matriz de celdas.

    Marca como especiales (`is_42 = True`) todas las celdas que forman
    parte del patrón y también las establece como visitadas (`visit = True`)
    para evitar que sean modificadas posteriormente por algoritmos
    de generación del laberinto.

    El patrón se coloca centrado dentro de la matriz.

    Args:
        matrix (list[list[Cell]]):
            Matriz bidimensional de objetos `Cell`.

        maze_width (int):
            Anchura total del laberinto.

        maze_height (int):
            Altura total del laberinto.

    Returns:
        list[list[Cell]]:
            La misma matriz recibida, modificada con las celdas
            pertenecientes al patrón "42".
    """
    if maze_width < WIDTH + 2 or maze_height < HEIGHT + 2:
        return matrix
    start_y = (maze_height - HEIGHT) // 2
    start_x = (maze_width - WIDTH) // 2
    for py in range(HEIGHT):
        for px in range(WIDTH):
            if PATTERN[py][px] == 1:
                cell = matrix[start_y + py][start_x + px]
                cell.is_42 = True
                cell.visit = True
    return matrix

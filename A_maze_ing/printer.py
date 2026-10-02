from cell import Cell
from colors import RESET, BOLD
from config_data import Map


def print_maze(
        grid: list[list[Cell]],
        palette: dict[str, str],
        game_map: Map,
        show_path: bool
) -> str:
    """
    Genera una representación en texto ASCII de un laberinto.

    La función recorre la cuadrícula de celdas y construye una cadena
    multilínea que representa visualmente el laberinto usando caracteres
    Unicode para paredes y distintos colores definidos en la paleta.

    Elementos especiales representados:
    - Entrada del laberinto ("ENT")
    - Salida del laberinto ("EXI")
    - Celdas marcadas como `is_42`
    - Camino resuelto (`is_path`) si `show_path` es True

    Args:
        grid (list[list[Cell]]):
            Matriz bidimensional de objetos `Cell` que contiene la
            estructura del laberinto y el estado de cada celda.

        palette (dict[str, str]):
            Diccionario con los códigos de color ANSI utilizados para
            pintar distintos elementos del laberinto.
            Claves esperadas:
            - "wall"
            - "entry"
            - "exit"
            - "c42"
            - "path"

        game_map (Map):
            Objeto que contiene la configuración del mapa, incluyendo:
            - width: ancho del laberinto
            - height: alto del laberinto
            - entry: coordenadas de entrada
            - exit_: coordenadas de salida

        show_path (bool):
            Indica si debe mostrarse el camino solucionado del laberinto.

    Returns:
        str:
            Cadena multilínea con la representación visual completa
            del laberinto lista para imprimirse en terminal.
    """
    lines: list[str] = []
    width = game_map.width
    height = game_map.height
    entry = game_map.entry
    exit_ = game_map.exit_

    for y in range(height):
        top = ""
        for x in range(width):
            cell = grid[y][x]
            top += palette["wall"] + "÷" + RESET
            top += (palette["wall"] + "---" + RESET) if cell.n else "   "
        top += palette["wall"] + "÷" + RESET
        lines.append(top)

        mid = ""
        for x in range(width):
            cell = grid[y][x]
            mid += (palette["wall"] + "│" + RESET) if cell.w else " "

            cx, cy = x, y
            if (cx, cy) == entry:
                mid += palette["entry"] + BOLD + "ENT" + RESET
            elif (cx, cy) == exit_:
                mid += palette["exit"] + BOLD + "EXI" + RESET
            elif cell.is_42:
                mid += palette["c42"] + "###" + RESET
            elif cell.is_path and show_path:
                mid += palette["path"] + " · " + RESET
            else:
                mid += "   "

        last = grid[y][width - 1]
        mid += (palette["wall"] + "│" + RESET) if last.e else "   "
        lines.append(mid)

    bottom = ""
    for x in range(width):
        cell = grid[height - 1][x]
        bottom += palette["wall"] + "÷" + RESET
        bottom += (palette["wall"] + "---" + RESET) if cell.s else "   "
    bottom += palette["wall"] + "÷" + RESET
    lines.append(bottom)

    return "\n".join(lines)

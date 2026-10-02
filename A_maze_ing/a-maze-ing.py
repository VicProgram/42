import sys
import os
from config_data import Map
from mazegen.generator import MazeGen
from pattern42 import HEIGHT as P42_HEIGHT
from pattern42 import WIDTH as P42_WIDTH
from colors import PALETTES
from printer import print_maze
from file_writer import write_maze_to_file

percent_imperfect = 0.55


def parsing(args: list[str]) -> bool:
    """
    Valida los argumentos recibidos por línea de comandos.

    El programa espera exactamente dos argumentos:
    - Nombre del script.
    - Archivo de configuración `config.txt`.

    Args:
        args (list[str]):
            Lista de argumentos obtenidos desde `sys.argv`.

    Returns:
        bool:
            `True` si los argumentos son válidos,
            `False` en caso contrario.
    """
    if len(args) != 2:
        return False

    if args[0] != "a-maze-ing.py" or args[1] != "config.txt":
        return False

    return True


def main() -> None:
    """
    Función principal del programa `A-Maze-ing`.

    Flujo principal:
    1. Valida los argumentos de entrada.
    2. Carga la configuración del laberinto.
    3. Genera y resuelve el laberinto.
    4. Muestra el laberinto en terminal.
    5. Guarda el resultado en un archivo.
    6. Ejecuta un menú interactivo para:
       - Regenerar el laberinto.
       - Mostrar u ocultar la solución.
       - Cambiar la paleta de colores.
       - Salir del programa.

    El programa también detecta si el tamaño del laberinto
    es demasiado pequeño para incluir el patrón especial "42".

    Returns:
        None:
            Esta función no devuelve ningún valor.
    """
    arguments = sys.argv

    if not parsing(arguments):
        print("Uso: python3 a_maze_ing.py config.txt")
        sys.exit(1)

    try:
        game_map = Map.from_file(arguments[1])
        print(f"Configuración cargada: {game_map.width}x{game_map.height}")
    except Exception as e:
        print(f"Error en la configuración: {e}")
        sys.exit(1)

    palette_index = 0
    show_path = True

    generator = MazeGen(game_map=game_map)
    cells = generator.generate(percent=percent_imperfect)
    short_path, route = generator.solve()

    maze_view = print_maze(cells, PALETTES[palette_index], game_map, show_path)
    os.system("clear")
    print(maze_view)
    if (
        game_map.width < P42_WIDTH + 2
        or game_map.height < P42_HEIGHT + 2
    ):
        print(
            "\nEl laberinto es demasiado "
            "pequeño para mostrar el patrón '42'"
        )
    write_maze_to_file(cells, game_map, route)

    while True:
        print("\n=== A-Maze-ing ===")
        print("1. Re-generate a new maze")
        print("2. Show/Hide path from entry to exit")
        print("3. Rotate maze colors")
        print("4. Quit")

        try:
            option = int(input("Choose? (1-4):"))
        except ValueError:
            os.system("clear")
            print(maze_view)
            print("\nPlease enter a valid number. (1-4)")
            continue
        match option:
            case 1:
                os.system("clear")
                generator = MazeGen(game_map=game_map, force_random=True)
                cells = generator.generate(percent=percent_imperfect)
                short_path, route = generator.solve()
                maze_view = print_maze(
                    cells, PALETTES[palette_index], game_map, show_path
                )
                print(maze_view)
                if (
                    game_map.width < P42_WIDTH + 2
                    or game_map.height < P42_HEIGHT + 2
                ):
                    print(
                        "\nEl laberinto es demasiado "
                        "pequeño para mostrar el patrón '42'"
                    )
                write_maze_to_file(cells, game_map, route)
            case 2:
                os.system("clear")
                show_path = not show_path
                print(
                    print_maze(
                        cells, PALETTES[palette_index], game_map, show_path
                    )
                )
                if (
                    game_map.width < P42_WIDTH + 2
                    or game_map.height < P42_HEIGHT + 2
                ):
                    print(
                        "\nEl laberinto es demasiado "
                        "pequeño para mostrar el patrón '42'"
                    )
            case 3:
                os.system("clear")
                palette_index = (palette_index + 1) % len(PALETTES)
                maze_view = print_maze(
                    cells, PALETTES[palette_index], game_map, show_path
                )
                print(maze_view)
                if (
                    game_map.width < P42_WIDTH + 2
                    or game_map.height < P42_HEIGHT + 2
                ):
                    print(
                        "\nEl laberinto es demasiado "
                        "pequeño para mostrar el patrón '42'"
                    )
            case 4:
                print("Exiting program.")
                sys.exit(0)
            case _:
                os.system("clear")
                maze_view = print_maze(
                    cells, PALETTES[palette_index], game_map, show_path
                )
                print(maze_view)
                print("\nInvalid option. Please choose again.")


if __name__ == "__main__":
    main()

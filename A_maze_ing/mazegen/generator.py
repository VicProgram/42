from collections import deque
import random
from cell import Cell
from config_data import Map
from pattern42 import apply_to_matrix
from typing import Optional, Tuple, List, Dict


class MazeGen:
    """
    Generador y resolvedor de laberintos.

    Esta clase crea laberintos a partir de una configuración `Map`
    y permite calcular el camino más corto entre entrada y salida.
    """
    def __init__(
        self,
        game_map: Map,
        seed: Optional[int] = None,
        force_random: bool = False,
    ) -> None:
        """
        Inicializa el generador de laberintos.

        Args:
            game_map (Map):
                Configuración del laberinto.

            seed (Optional[int], optional):
                Semilla aleatoria personalizada.

            force_random (bool, optional):
                Si es `True`, ignora cualquier semilla fija.
        """
        self.game_map: Map = game_map
        self.matrix: Optional[List[List[Cell]]] = None
        self.generation_route: Optional[List[str]] = None
        self.shortest_path: Optional[
            Tuple[List[Tuple[int, int]], List[str]]
        ] = None

        if force_random:
            random.seed()
        else:
            actual_seed = seed if seed is not None else game_map.seed
            if actual_seed is not None:
                random.seed(actual_seed)
            else:
                random.seed()

    def _wall_breaker(self, curr: Cell, neigh: Cell, direction: str) -> None:
        """
        Elimina la pared entre dos celdas conectadas.

        Args:
            curr (Cell):
                Celda actual.

            neigh (Cell):
                Celda vecina.

            direction (str):
                Dirección de conexión (`N`, `S`, `E`, `W`).
        """
        if direction == "N":
            curr.n = 0
            neigh.s = 0
        elif direction == "S":
            curr.s = 0
            neigh.n = 0
        elif direction == "E":
            curr.e = 0
            neigh.w = 0
        elif direction == "W":
            curr.w = 0
            neigh.e = 0

    def _break_random(self, percent: float = 0.95) -> None:
        """
        Rompe paredes aleatorias para crear ciclos en el laberinto.

        Args:
            percent (float, optional):
                Cantidad aproximada de paredes a eliminar.
        """

        assert self.matrix is not None
        moves = [(0, -1, "N"), (0, 1, "S"), (-1, 0, "W"), (1, 0, "E")]

        num_of_walls = int(
            (self.game_map.width * self.game_map.height) * percent
        )

        for _ in range(num_of_walls):
            try:
                x = random.randint(1, self.game_map.width - 2)
                y = random.randint(1, self.game_map.height - 2)
            except ValueError:
                x = random.randint(1, self.game_map.width - 1)
                y = random.randint(1, self.game_map.height - 1)
                continue
            curr_cell = self.matrix[y][x]
            if curr_cell.is_42:
                continue

            dx, dy, direction = random.choice(moves)
            nx, ny = x + dx, y + dy
            neigh_cell = self.matrix[ny][nx]

            if not neigh_cell.is_42:
                self._wall_breaker(curr_cell, neigh_cell, direction)

    def generate(self, percent: float = 0.15) -> List[List[Cell]]:
        """
        Genera un nuevo laberinto.

        Usa backtracking para crear caminos válidos evitando
        las celdas del patrón "42".

        Args:
            percent (float, optional):
                Porcentaje de paredes aleatorias eliminadas
                si el laberinto no es perfecto.

        Returns:
            List[List[Cell]]:
                Matriz final del laberinto.
        """
        width = self.game_map.width
        height = self.game_map.height

        raw_matrix = []
        for i in range(height):
            row = []
            for j in range(width):
                row.append(Cell(x=j, y=i))
            raw_matrix.append(row)

        self.matrix = apply_to_matrix(raw_matrix, width, height)

        moves = [(0, -1, "N"), (0, 1, "S"), (-1, 0, "W"), (1, 0, "E")]
        maze_path: List[Tuple[int, int]] = []
        final_path: List[Tuple[int, int]] = []

        start_x, start_y = self.game_map.entry
        exit_x, exit_y = self.game_map.exit_
        self.matrix[start_y][start_x].visit = True
        maze_path.append((start_x, start_y))

        while len(maze_path) > 0:
            cx, cy = maze_path[-1]
            curr_cell = self.matrix[cy][cx]
            if cx == exit_x and cy == exit_y and not final_path:
                final_path = list(maze_path)

            valid_neigh = []
            for dx, dy, direction in moves:
                nx, ny = cx + dx, cy + dy

                if 0 <= nx < width and 0 <= ny < height:
                    neigh_cell = self.matrix[ny][nx]
                    if not neigh_cell.visit and not neigh_cell.is_42:
                        valid_neigh.append((nx, ny, direction))
            if valid_neigh:
                curr_cell.visit = True
                next_x, next_y, direction = random.choice(valid_neigh)
                next_cell = self.matrix[next_y][next_x]
                self._wall_breaker(curr_cell, next_cell, direction)
                next_cell.visit = True
                maze_path.append((next_x, next_y))
            else:
                maze_path.pop()

            self.generation_route = []
            for i in range(len(final_path) - 1):
                x1, y1 = final_path[i]
                x2, y2 = final_path[i + 1]
                if y2 < y1:
                    self.generation_route.append("N")
                elif y2 > y1:
                    self.generation_route.append("S")
                elif x2 > x1:
                    self.generation_route.append("E")
                elif x2 < x1:
                    self.generation_route.append("W")
        if not self.game_map.perfect:
            self._break_random(percent=percent)
        return self.matrix

    def solve(self) -> Tuple[List[Tuple[int, int]], List[str]]:
        """
        Resuelve el laberinto usando búsqueda BFS.

        Calcula el camino más corto entre la entrada y la salida
        y marca las celdas pertenecientes a la solución.

        Returns:
            Tuple[List[Tuple[int, int]], List[str]]:
                - Lista de coordenadas del camino.
                - Lista de movimientos (`N`, `S`, `E`, `W`).

        Raises:
            ValueError:
                Si el laberinto aún no ha sido generado.
        """
        if not self.matrix:
            raise ValueError("The lab doesn't exist")
        moves = [(0, -1, "N"), (0, 1, "S"), (-1, 0, "W"), (1, 0, "E")]
        start = self.game_map.entry
        target = self.game_map.exit_
        cola = deque([start])
        visited = {start}
        parents: Dict[Tuple[int, int], Tuple[int, int]] = {}
        found = False
        while cola:
            cx, cy = cola.popleft()
            if (cx, cy) == target:
                found = True
                break
            curr_cell = self.matrix[cy][cx]
            for dx, dy, direction in moves:
                nx, ny = cx + dx, cy + dy
                if (0 <= nx < self.game_map.width and
                   0 <= ny < self.game_map.height):
                    if (nx, ny) in visited:
                        continue
                    neigh_cell = self.matrix[ny][nx]
                    if neigh_cell.is_42:
                        continue
                    if (
                        (direction == "N" and curr_cell.n == 0)
                        or (direction == "S" and curr_cell.s == 0)
                        or (direction == "E" and curr_cell.e == 0)
                        or (direction == "W" and curr_cell.w == 0)
                    ):
                        visited.add((nx, ny))
                        parents[(nx, ny)] = (cx, cy)
                        cola.append((nx, ny))
        path: List[Tuple[int, int]] = []
        route: List[str] = []
        if found:
            curr = target
            while curr != start:
                path.append(curr)
                curr = parents[curr]
            path.append(start)
            path.reverse()

            for i in range(len(path) - 1):
                x1, y1 = path[i]
                x2, y2 = path[i + 1]
                if y2 < y1:
                    route.append("N")
                elif y2 > y1:
                    route.append("S")
                elif x2 > x1:
                    route.append("E")
                elif x2 < x1:
                    route.append("W")
        if self.matrix:
            for row in self.matrix:
                for cell in row:
                    cell.is_path = False
            for x, y in path:
                self.matrix[y][x].is_path = True
        self.shortest_path = (path, route)
        return self.shortest_path

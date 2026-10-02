class Cell:
    """
    Representa una celda individual dentro del laberinto.

    Cada celda almacena información sobre:
    - La existencia de paredes en sus cuatro lados.
    - Su posición dentro de la matriz.
    - Su estado de visita.
    - Si pertenece al patrón especial "42".
    - Si forma parte del camino solución.

    Attributes:
        n (int):
            Estado de la pared norte.
            `1` indica pared presente, `0` ausencia de pared.

        s (int):
            Estado de la pared sur.

        w (int):
            Estado de la pared oeste.

        e (int):
            Estado de la pared este.

        y (int):
            Coordenada vertical de la celda.

        x (int):
            Coordenada horizontal de la celda.

        visit (bool):
            Indica si la celda ha sido visitada durante
            la generación o resolución del laberinto.

        is_42 (bool):
            Indica si la celda pertenece al patrón especial "42".

        is_path (bool):
            Indica si la celda forma parte del camino solución.
    """
    def __init__(
        self,
        n: int = 1,
        s: int = 1,
        w: int = 1,
        e: int = 1,
        visit: bool = False,
        is_42: bool = False,
        is_path: bool = False,
        y: int = 0,
        x: int = 0,
    ) -> None:
        self.n = n
        self.s = s
        self.w = w
        self.e = e
        self.y = y
        self.x = x
        self.visit = visit
        self.is_42 = is_42
        self.is_path = is_path

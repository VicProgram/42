from dataclasses import dataclass
from typing import Optional
import random
from pattern42 import is_42_cell


def _parse_coord(raw: str, key: str) -> tuple[int, int]:
    """
    Convierte una cadena con coordenadas en una tupla de enteros.

    El formato esperado es:
        "x,y"

    La función valida que:
    - Existan exactamente dos valores.
    - Ambos valores sean enteros válidos.

    Args:
        raw (str):
            Cadena que contiene las coordenadas separadas por coma.

        key (str):
            Nombre de la clave de configuración utilizada para
            generar mensajes de error más descriptivos.

    Returns:
        tuple[int, int]:
            Tupla con las coordenadas convertidas a enteros.

    Raises:
        ValueError:
            Si el formato es inválido o contiene valores no enteros.
    """
    parts = [p.strip() for p in raw.split(",")]

    if len(parts) != 2:
        raise ValueError(
            f"'{key}' debe tener exactamente dos valores separados por coma "
            f"(ej. '0, 0'), se recibió: '{raw}'"
        )

    coords = []

    for part in parts:
        if not part.lstrip("-").isdigit():
            raise ValueError(
                f"'{key}' contiene un valor no entero: '{part}'"
            )

        coords.append(int(part))

    return coords[0], coords[1]


ALLOWED_KEYS = {"WIDTH", "HEIGHT", "ENTRY", "EXIT",
                "OUTPUT_FILE", "PERFECT", "SEED"}


def config_data() -> dict[str, str]:
    """
    Lee y valida el archivo de configuración `config.txt`.

    La función procesa el archivo línea por línea:
    - Ignora líneas vacías y comentarios.
    - Verifica el formato `CLAVE=VALOR`.
    - Comprueba que las claves sean válidas.
    - Evita claves duplicadas.

    Returns:
        dict[str, str]:
            Diccionario con las claves y valores leídos del archivo.

    Raises:
        FileNotFoundError:
            Si el archivo `config.txt` no existe.

        ValueError:
            Si el formato del archivo es incorrecto, existen claves
            desconocidas o duplicadas.
    """
    config: dict[str, str] = {}
    try:
        with open("config.txt", "r") as file:
            for line_number, line in enumerate(file, start=1):
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                if "=" not in line:
                    raise ValueError(f"Invalid format in line {line_number}")

                key, value = line.split("=", 1)
                key = key.strip()
                value = value.strip()

                if key not in ALLOWED_KEYS:
                    raise ValueError(
                        f"Clave desconocida '{key}' en la "
                        f"línea {line_number}."
                        "Las claves permitidas son: "
                        f"{', '.join(sorted(ALLOWED_KEYS))}"
                    )

                if key in config:
                    raise ValueError(
                        f"Duplicated key '{key}' in line {line_number}"
                    )
                config[key] = value
        return config
    except FileNotFoundError:
        raise FileNotFoundError("Archivo 'config.txt' no encontrado.")


@dataclass
class Map:
    """
    Representa la configuración completa de un laberinto.

    Attributes:
        width (int):
            Anchura del laberinto.

        height (int):
            Altura del laberinto.

        entry (tuple[int, int]):
            Coordenadas de entrada.

        exit_ (tuple[int, int]):
            Coordenadas de salida.

        perfect (bool):
            Indica si el laberinto debe ser perfecto
            (sin ciclos).

        output_file (str):
            Ruta del archivo donde se guardará el laberinto.

        seed (Optional[int]):
            Semilla utilizada para generar resultados aleatorios
            reproducibles.
    """
    width: int
    height: int
    entry: tuple[int, int]
    exit_: tuple[int, int]
    perfect: bool
    output_file: str
    seed: Optional[int]

    def __post_init__(self) -> None:
        """
        Valida la configuración del laberinto tras la inicialización.

        Validaciones realizadas:
        - Tamaño del laberinto.
        - Coordenadas dentro de límites.
        - Entrada y salida distintas.
        - Entrada y salida fuera del patrón "42".
        - Archivo de salida con extensión `.txt`.

        También inicializa la semilla aleatoria.

        Raises:
            ValueError:
                Si alguno de los parámetros es inválido.
        """

        random.seed(self.seed)

        if (self.width < 2 or self.width > 100 or
           self.height < 2 or self.height > 100):
            raise ValueError(
                "WIDTH and HEIGHT must be between 3 and 100. "
                f"Received WIDTH={self.width}, HEIGHT={self.height}"
            )

        ex, ey = self.entry
        if not (0 <= ex < self.width and 0 <= ey < self.height):
            raise ValueError(
                f"ENTRY {self.entry} is out of bounds for maze "
                f"({self.width}x{self.height}). "
                f"Valid range: x ∈ [0, {self.width-1}],"
                f"    y ∈ [0, {self.height-1}]"
            )

        ox, oy = self.exit_
        if not (0 <= ox < self.width and 0 <= oy < self.height):
            raise ValueError(
                f"EXIT {self.exit_} is out of bounds for maze "
                f"({self.width}x{self.height}). "
                f"Valid range: x ∈ [0, {self.width-1}],"
                f" y ∈ [0, {self.height-1}]"
            )

        if self.entry == self.exit_:
            raise ValueError(
                 f"ENTRY and EXIT cannot be the same cell: {self.entry}"
            )

        if is_42_cell(ex, ey, self.width, self.height):
            raise ValueError(
                f"ENTRY {self.entry} falls on a '42' pattern cell. "
                f"Choose a different position."
            )

        if is_42_cell(ox, oy, self.width, self.height):
            raise ValueError(
                f"EXIT {self.exit_} falls on a '42' pattern cell. "
                f"Choose a different position."
            )

        if not self.output_file.endswith(".txt"):
            raise ValueError(
                f"OUTPUT_FILE must be a .txt file, "
                f"got: '{self.output_file}'"
            )

    @classmethod
    def from_file(cls, path: str = "config.txt") -> "Map":
        """
        Crea una instancia de `Map` a partir de un archivo de configuración.

        La función:
        - Lee el archivo de configuración.
        - Verifica que existan todas las claves requeridas.
        - Convierte los valores al tipo adecuado.
        - Valida los datos antes de crear el objeto.

        Args:
            path (str, optional):
                Ruta del archivo de configuración.
                Por defecto `"config.txt"`.

        Returns:
            Map:
                Instancia completamente validada de `Map`.

        Raises:
            KeyError:
                Si faltan claves obligatorias.

            ValueError:
                Si alguno de los valores tiene un formato inválido.
        """
        data = config_data()

        required = [
            "WIDTH", "HEIGHT", "ENTRY", "EXIT", "PERFECT", "OUTPUT_FILE"
        ]

        missing = [k for k in required if k not in data]
        if missing:
            raise KeyError(
                f"Missing required keys in config.txt: {', '.join(missing)}"
            )

        try:
            width = int(data["WIDTH"])
        except ValueError:
            raise ValueError(
                f"WIDTH must be an integer, got: '{data['WIDTH']}'"
            )

        try:
            height = int(data["HEIGHT"])
        except ValueError:
            raise ValueError(
                f"HEIGHT must be an integer, got: '{data['HEIGHT']}'"
            )

        entry = _parse_coord(data["ENTRY"], "ENTRY")
        exit_ = _parse_coord(data["EXIT"], "EXIT")

        perfect_raw = data["PERFECT"].lower()
        if perfect_raw not in ("true", "false"):
            raise ValueError(
                f"PERFECT must be 'true' or 'false', got: '{data['PERFECT']}'"
            )

        perfect = perfect_raw == "true"

        seed = None
        if "SEED" in data:
            try:
                seed = int(data["SEED"])
            except ValueError:
                raise ValueError(
                    f"SEED must be an integer, got: '{data['SEED']}'"
                )

        return cls(
            width=width,
            height=height,
            entry=entry,
            exit_=exit_,
            perfect=perfect,
            output_file=data["OUTPUT_FILE"],
            seed=seed
        )

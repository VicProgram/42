"""End-to-end and unit tests for the simulation engine."""

import re

import pytest

from models import Connection, DroneMap, Hub, ReservationTable, Solver
from parsing import Parser

ANSI = re.compile(r"\x1b\[[0-9;]*m")

LINEAR_MAP = """\
nb_drones: 2

start_hub: start 0 0 [color=green]
hub: waypoint1 1 0 [color=blue]
hub: waypoint2 2 0 [color=blue]
end_hub: goal 3 0 [color=red]

connection: start-waypoint1
connection: waypoint1-waypoint2
connection: waypoint2-goal
"""

FORK_MAP = """\
nb_drones: 4

start_hub: start 0 0 [color=green]
hub: junction 1 0 [color=yellow max_drones=2]
hub: path_a 2 1 [color=blue]
hub: path_b 2 -1 [color=blue]
end_hub: goal 3 0 [color=red max_drones=3]

connection: start-junction [max_link_capacity=2]
connection: junction-path_a
connection: junction-path_b
connection: path_a-goal
connection: path_b-goal
"""

RESTRICTED_MAP = """\
nb_drones: 1

start_hub: start 0 0 [color=green]
hub: slow 1 0 [zone=restricted]
end_hub: goal 2 0 [color=red]

connection: start-slow
connection: slow-goal
"""

UNREACHABLE_MAP = """\
nb_drones: 1

start_hub: start 0 0
hub: island 1 1
end_hub: goal 5 5

connection: start-island
"""


def load(tmp_path, text):
    """Parse a map from a temporary file."""
    path = tmp_path / "map.txt"
    path.write_text(text, encoding="utf-8")
    drone_map = DroneMap()
    parser = Parser(drone_map)
    parser.parse_file(str(path))
    return drone_map, parser.nb_drones


def run_and_capture(tmp_path, capsys, text, drones):
    """Run the solver and return its output without ANSI colour codes."""
    drone_map, _ = load(tmp_path, text)
    Solver(drone_map, drones).run()
    return drone_map, ANSI.sub("", capsys.readouterr().out)


def turns(output):
    """Return the non-empty output lines (one per simulation turn)."""
    return [line for line in output.splitlines() if line.strip()]


def test_linear_map_reaches_goal(tmp_path, capsys):
    drone_map, output = run_and_capture(tmp_path, capsys, LINEAR_MAP, 2)

    assert len(turns(output)) >= 3
    assert set(re.findall(r"D(\d+)-goal", output)) == {"1", "2"}


def test_fork_map_routes_all_drones_to_goal(tmp_path, capsys):
    _, output = run_and_capture(tmp_path, capsys, FORK_MAP, 4)

    reached = set(re.findall(r"D(\d+)-goal", output))
    assert reached == {"1", "2", "3", "4"}


def test_hop_costs_two_turns_for_restricted_zone(tmp_path, capsys):
    _, output = run_and_capture(tmp_path, capsys, RESTRICTED_MAP, 1)

    lines = turns(output)
    assert "D1-start-slow" in lines[0]
    assert "D1-slow" in lines[1]


def test_output_never_exceeds_hub_or_link_capacity(tmp_path, capsys):
    drone_map, output = run_and_capture(tmp_path, capsys, FORK_MAP, 4)

    for line in turns(output):
        hub_occupancy = {}
        link_occupancy = {}

        for token in line.split():
            match = re.fullmatch(r"D\d+-(.+)", token)
            assert match, f"unexpected token: {token!r}"
            body = match.group(1)

            if "-" in body:
                first, second = body.split("-", 1)
                link_occupancy[frozenset((first, second))] = (
                    link_occupancy.get(frozenset((first, second)), 0) + 1
                )
            else:
                hub_occupancy[body] = hub_occupancy.get(body, 0) + 1

        for name, count in hub_occupancy.items():
            hub = drone_map.hubs[name]
            if hub.hub_type not in ("start", "end"):
                assert count <= hub.max_drones

        for endpoints, count in link_occupancy.items():
            connection = next(
                conn for conn in drone_map.connections
                if conn._key() == endpoints
            )
            assert count <= connection.capacity


def test_unreachable_end_hub_exits_with_error(tmp_path, capsys):
    drone_map, drones = load(tmp_path, UNREACHABLE_MAP)

    with pytest.raises(SystemExit) as excinfo:
        Solver(drone_map, drones).run()

    assert excinfo.value.code == 1
    assert "unreachable" in capsys.readouterr().err.lower()


def test_move_costs_follow_zone_type():
    solver = object.__new__(Solver)

    normal = Hub("n", 0, 0, "normal", "none", "normal", 1)
    priority = Hub("p", 0, 0, "priority", "none", "normal", 1)
    restricted = Hub("r", 0, 0, "restricted", "none", "normal", 1)
    blocked = Hub("b", 0, 0, "blocked", "none", "normal", 1)

    assert solver.get_move_costs(normal) == 1
    assert solver.get_move_costs(priority) == 1
    assert solver.get_move_costs(restricted) == 2
    assert solver.get_move_costs(blocked) == 999999


def test_reservation_table_enforces_hub_capacity():
    hub = Hub("h", 0, 0, "normal", "none", "normal", 1)
    start = Hub("s", 0, 0, "normal", "none", "start", 1)
    table = ReservationTable()

    assert table.hub_available(hub, 1)
    table.reserve_hub("h", 1)

    assert not table.hub_available(hub, 1)
    assert table.hub_available(hub, 2)

    table.reserve_hub("s", 1)
    assert table.hub_available(start, 1)

    table.clear()
    assert table.hub_available(hub, 1)


def test_reservation_table_enforces_link_capacity():
    first = Hub("a", 0, 0)
    second = Hub("b", 1, 0)
    link = Connection("l", first, second, 1)
    wide_link = Connection("w", first, second, 2)
    table = ReservationTable()

    assert table.link_available(link, 1)
    table.reserve_link(link, 1)

    assert not table.link_available(link, 1)
    assert table.link_available(wide_link, 1)
    assert table.link_available(link, 2)

    table.clear()
    assert table.link_available(link, 1)

"""Unit tests for the map parser."""

import pytest

from models import DroneMap
from parsing import Parser

LINEAR_MAP = """\
# Linear path: 2 drones, 3 hops
nb_drones: 2

start_hub: start 0 0 [color=green]
hub: waypoint1 1 0 [color=blue]
hub: waypoint2 2 0 [color=blue]
end_hub: goal 3 0 [color=red]

connection: start-waypoint1
connection: waypoint1-waypoint2
connection: waypoint2-goal
"""

METADATA_MAP = """\
nb_drones: 3

start_hub: start 0 0 [color=green max_drones=2]
hub: rest 1 0 [zone=restricted]
hub: prio 2 0 [zone=priority max_drones=3]
hub: blocked 2 1 [zone=blocked]
end_hub: goal 3 0 [color=red]

connection: start-rest [max_link_capacity=2]
connection: rest-prio
"""


def parse(tmp_path, text, name="map.txt"):
    """Parse a map written to a temporary file and return the result."""
    path = tmp_path / name
    path.write_text(text, encoding="utf-8")
    drone_map = DroneMap()
    parser = Parser(drone_map)
    parser.parse_file(str(path))
    return drone_map, parser


def test_parses_hubs_connections_and_drone_count(tmp_path):
    drone_map, parser = parse(tmp_path, LINEAR_MAP)

    assert parser.nb_drones == 2
    assert list(drone_map.hubs) == [
        "start", "waypoint1", "waypoint2", "goal"
    ]
    assert len(drone_map.connections) == 3
    assert drone_map.start_hub is drone_map.hubs["start"]
    assert drone_map.end_hub is drone_map.hubs["goal"]
    assert drone_map.start_hub.hub_type == "start"
    assert drone_map.end_hub.hub_type == "end"


def test_parses_metadata_and_defaults(tmp_path):
    drone_map, parser = parse(tmp_path, METADATA_MAP)

    assert parser.nb_drones == 3
    assert drone_map.hubs["rest"].zone_type == "restricted"
    assert drone_map.hubs["prio"].zone_type == "priority"
    assert drone_map.hubs["prio"].max_drones == 3
    assert drone_map.hubs["blocked"].zone_type == "blocked"
    assert drone_map.hubs["rest"].max_drones == 1
    assert drone_map.hubs["start"].color == "green"
    assert drone_map.connections[0].capacity == 2
    assert drone_map.connections[1].capacity == 1


def test_ignores_comments_and_blank_lines(tmp_path):
    drone_map, parser = parse(tmp_path, LINEAR_MAP)

    assert parser.hub_counter == 4
    assert parser.connection_counter == 3


def test_rejects_hub_before_nb_drones(tmp_path):
    text = "hub: a 0 0\nnb_drones: 1\n"
    with pytest.raises(ValueError, match="first valid data line"):
        parse(tmp_path, text)


def test_rejects_duplicate_nb_drones(tmp_path):
    text = "nb_drones: 1\nnb_drones: 2\n"
    with pytest.raises(ValueError, match="Duplicate 'nb_drones"):
        parse(tmp_path, text)


def test_rejects_non_positive_drone_count(tmp_path):
    text = "nb_drones: 0\n"
    with pytest.raises(ValueError, match="Invalid drone number"):
        parse(tmp_path, text)


def test_rejects_duplicate_hub(tmp_path):
    text = "nb_drones: 1\nhub: a 0 0\nhub: a 1 1\n"
    with pytest.raises(ValueError, match="already exists"):
        parse(tmp_path, text)


def test_rejects_missing_start_hub(tmp_path):
    text = "nb_drones: 1\nend_hub: goal 0 0\n"
    with pytest.raises(ValueError, match="no start_hub"):
        parse(tmp_path, text)


def test_rejects_missing_end_hub(tmp_path):
    text = "nb_drones: 1\nstart_hub: start 0 0\n"
    with pytest.raises(ValueError, match="no end_hub"):
        parse(tmp_path, text)


def test_rejects_connection_to_unknown_hub(tmp_path):
    text = (
        "nb_drones: 1\n"
        "start_hub: start 0 0\n"
        "end_hub: goal 1 0\n"
        "connection: start-ghost\n"
    )
    with pytest.raises(ValueError, match="Zone not found"):
        parse(tmp_path, text)


def test_rejects_self_connection(tmp_path):
    text = (
        "nb_drones: 1\n"
        "start_hub: start 0 0\n"
        "end_hub: goal 1 0\n"
        "connection: start-start\n"
    )
    with pytest.raises(ValueError, match="cannot link a hub to itself"):
        parse(tmp_path, text)


def test_rejects_duplicate_connection(tmp_path):
    text = (
        "nb_drones: 1\n"
        "start_hub: start 0 0\n"
        "end_hub: goal 1 0\n"
        "connection: start-goal\n"
        "connection: goal-start\n"
    )
    with pytest.raises(ValueError, match="already exists"):
        parse(tmp_path, text)


def test_rejects_unknown_zone_type(tmp_path):
    text = "nb_drones: 1\nstart_hub: start 0 0 [zone=warp]\n"
    with pytest.raises(ValueError, match="Invalid zone type"):
        parse(tmp_path, text)


def test_rejects_forbidden_metadata_key(tmp_path):
    text = "nb_drones: 1\nstart_hub: start 0 0 [speed=3]\n"
    with pytest.raises(ValueError, match="Unknown or forbidden"):
        parse(tmp_path, text)


def test_rejects_malformed_metadata(tmp_path):
    text = "nb_drones: 1\nstart_hub: start 0 0 [color=red] [zone=normal]\n"
    with pytest.raises(ValueError, match="Malformed metadata"):
        parse(tmp_path, text)


def test_rejects_invalid_coordinates(tmp_path):
    text = "nb_drones: 1\nhub: a zero 0\n"
    with pytest.raises(ValueError, match="Invalid coordinates"):
        parse(tmp_path, text)


def test_rejects_hyphen_in_hub_name(tmp_path):
    text = "nb_drones: 1\nhub: my-hub 0 0\n"
    with pytest.raises(ValueError, match="contains '-'"):
        parse(tmp_path, text)


def test_rejects_unknown_line_syntax(tmp_path):
    text = "nb_drones: 1\nnonsense: value\n"
    with pytest.raises(ValueError, match="Unknown structure or syntax"):
        parse(tmp_path, text)


def test_reports_line_number_on_error(tmp_path):
    text = "nb_drones: 1\n\nhub: a 0 0\nhub: a 1 1\n"
    with pytest.raises(ValueError, match="Line 4:"):
        parse(tmp_path, text)


def test_missing_file_raises_value_error(tmp_path):
    drone_map = DroneMap()
    parser = Parser(drone_map)

    with pytest.raises(ValueError, match="Error opening file"):
        parser.parse_file(str(tmp_path / "does-not-exist.txt"))

def bracket_validator(s: str) -> bool:
    if not isinstance(s, str):
        return False
    
    cpy = s[:]

    
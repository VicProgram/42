def bracket_validator(s: str) -> bool:

    if not isinstance(s, str):
        return False
    
    l = ["(", ")", "{", "}", "[", "]"]

    for c in s:
        if c not in l:
            s.replace(c, "")
        s.replace("()", "")
        s.replace("[]", "")
        s.replace("{}", "")
    return s == ""
print(bracket_validator("()]{}"))
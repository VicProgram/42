def bracket_validator(s: str) -> bool:
    l =["(", ")", "[", "]", "{", "}"]

    for c in s:
        if c not in l:
            s = s.replace(c, "")
        s = s.replace("()", "")
        s = s.replace("[]", "")
        s = s.replace("{}", "")
    return s == ""
    

print(bracket_validator("()[]{}"))
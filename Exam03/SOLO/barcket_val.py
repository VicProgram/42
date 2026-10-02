def bracket_validator(s: str) -> bool:
    
    l = {")": "(", "}": "{", "]": "["}
    res = []
    
    for c in s:
        if c in l.values():
            res.append(c)

        elif c in l.keys():
            if not res or res.pop() != l[c]:
                return False
    return len(res) == 0


print(bracket_validator("()[]{}"))
print(bracket_validator("((({})))[]{}"))
print(bracket_validator("hello(world)[test]{code}"))
print(bracket_validator("hello(world)[test]{code})"))



            # Es un símbolo de apertura, lo agregamos a la res
            # Es un símbolo de cierre, verificamos la res
            # Faltó el símbolo de apertura o no coinciden
            # Si la pila está vacía, todos los símbolos se cerraron correctamente
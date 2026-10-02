def whisper_cipher(text: str, shift: int) -> str:
    minus ="abcdefghijklmnopqrstuvwxyz"
    mayus = minus.upper()
    
    if not isinstance(text, str):
        return False
    if not isinstance(shift, int):
        return False
    res = ""

    for c in text:
        if c.isalpha():
            if c in minus:
                index = minus.index(c)
                index += shift
                c = minus[index % 26]
                res += c
            if c in mayus:
                index = mayus.index(c)
                index += shift
                c = mayus[index % 26]
                res += c 
        else:
            res += c
    return res

print(whisper_cipher("abc1234xyz", 3))
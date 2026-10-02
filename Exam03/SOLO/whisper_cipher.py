def whisper_cipher(text: str, shift: int) -> str:
    if not isinstance(text, str):
        return 'ERROR'
    if not isinstance(shift, int):
        return 'ERROR'
    
    may = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    minus = may.lower()
    res = ""

    for c in text:
        if c.isalpha():
            if c in may:
                ind = may.index(c)
                ind += shift
                c = may[ind % 26]
                res += c
            if c in minus:
                ind = minus.index(c)
                ind += shift
                c = minus[ind % 26]
                res += c
        else:
            res += c
    return res


print(whisper_cipher("khoor", 3))
print(whisper_cipher("xyz", 3))
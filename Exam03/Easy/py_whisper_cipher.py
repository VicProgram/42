def whisper_cipher(text: str, shift: int) -> str:
        
    res = ""
    for c in text:
        if c.isalpha():
            if c.islower():
                start = ord('a')
            else:
                start = ord('A')
            newch = chr((ord(c) - start + shift) % 26 + start)
            res += newch
        else:
            res += c

    return res


print(whisper_cipher("xyzabc123", 1))
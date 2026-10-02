def whisper_cipher(text: str, shift: int) -> str:

    if not isinstance(text, str):
        return 'ERROR'
    if not isinstance(shift, int):
        return 'ERROR'

    res = ""
    for c in text:
        if c.isalpha():
            if c.islower():
                start = ord('a')
            else:
                start = ord('A')
            newc = chr((ord(c) - start + shift ) % 26 + start)
            res += newc
        else:
            res += c
    return res


print(whisper_cipher("hello", 3))
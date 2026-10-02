def string_sculptor(text: str) -> str:
    res = ""
    ind = 0

    for char in text:
        if char.isalpha():
            if ind % 2 == 0:
                res += char.lower()
            else:
                res += char.upper()
            
            ind += 1
        else:
            res += char
    return res


print(string_sculptor("mi MOno AmeLio Y Yo"))
print(string_sculptor("hola mundo 3"))
print(string_sculptor("hello"))
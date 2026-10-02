def echo_validator(text: str) -> bool:
    if not isinstance(text, str):
        return False
    clean = "".join(char.lower() for char in text if char.isalpha())
    if clean == "":
        return False
    return clean == clean[::-1]

print (echo_validator("A man a plan a canal Panama"))
print (echo_validator(""))
print (echo_validator(5))

# cuidado con el try except
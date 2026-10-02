def echo_validator(text: str) -> bool:

    if not isinstance(text, str):
        return False
    
    new = text.lower().replace(" ", "")
    
    return new == new[::-1]


print(echo_validator("Was it a car or a cat I saw"))


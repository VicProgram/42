def echo_validator(text: str) -> bool:

    if not isinstance(text, str):
        return False
    
    new = text[:].lower().replace(" ", "")

    
    return new == text[::-1]

print(echo_validator("mamam"))
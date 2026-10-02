def echo_validator(text: str) -> bool:
    if not isinstance(text, str):
        return False

    mia = text.lower().replace(" ", "")
    tuya = mia[::-1]
    return  tuya == mia


print(echo_validator("race a ecar"))
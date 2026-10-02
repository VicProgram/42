def pattern_tracker(text: str) -> int:

    ctr = 0

    for a, b in zip(text, text[1:]):
        if a.isdigit() and b.isdigit() and int(b) == int(a) + 1:
            ctr +=1
    return ctr

print(pattern_tracker("12a34"))
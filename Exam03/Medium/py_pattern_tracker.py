def pattern_tracker(text: str) -> int:
    ctr = 0

    for a, b in zip(text, text[1:]):
        if a.isdigit() and b.isdigit() and int(b) == int(a) + 1:
            ctr += 1

    return ctr



print(pattern_tracker("012a34"))
print(pattern_tracker("01234567"))
print(pattern_tracker("abc"))
print(pattern_tracker("1a2b3c4"))
print(pattern_tracker("112233"))

def cryptic_sorter(strings: list[str]) -> list[str]:

    def count_vocals(chain: str) -> int:
        vocals = "aeiou"
        return sum(1 for voc in chain.lower() if voc in vocals)
    
    return sorted(
        strings, key=lambda s: (len(s), s.lower(), count_vocals(s))
    )


print(cryptic_sorter(["apple", "cat", "banana", "dog", "elephant"]))
def twist_sequence(arr: list[int], k: int) -> list[int]:
    
    for i in range(k):
        c = arr.pop()
        arr.insert(0, c)
    return arr

print(twist_sequence([1, 2, 3, 4, 5], 2))
def shadow_merge(list1: list[int], list2: list[int]) -> list[int]:
    res = []

    for c in list1:
        res.append(c)
    for c in list2:
        res.append(c)

    return sorted(res)

print(shadow_merge([], [5, 2, 4, 6]))
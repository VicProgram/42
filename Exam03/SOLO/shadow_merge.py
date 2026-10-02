def shadow_merge(list1: list[int], list2: list[int]) -> list[int]:
    res = []

    for l in list1:
        res.append(l)
    for l in list2:
        res.append(l)
    return res


print(shadow_merge([1, 3, 5], [2, 4, 6]))
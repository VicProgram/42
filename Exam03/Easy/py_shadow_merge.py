def shadow_merge(list1: list[int], list2: list[int]) -> list[int]:

    res = []

    for c in list1:
        res.append(c)
    for c in list2:
        res.append(c)
    res.sort()
    return res


print(shadow_merge([1, 2, 7, 5, 0, 4], [2, 3, 4]))
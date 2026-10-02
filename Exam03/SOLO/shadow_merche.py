def shadow_merge(list1: list[int], list2: list[int]) -> list[int]:
    res = []

    res += list1
    res += list2

    return sorted(res)

print(shadow_merge([1, 3, 5], [2, 4, 6]))
print(shadow_merge([], []))
def mirror_matrix(matrix: list[list[int]]) -> list[list[int]]:
    res = matrix[:]

    for mat in res:
       mat.reverse()
    return res


print(mirror_matrix([[1, 2, 3], [4, 5, 6]]))
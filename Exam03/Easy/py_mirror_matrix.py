def mirror_matrix(matrix: list[list[int]]) -> list[list[int]]:
    newmat = [fila[::-1] for fila in matrix]
    return newmat



print(mirror_matrix([[1, 2, 3, 4]]))
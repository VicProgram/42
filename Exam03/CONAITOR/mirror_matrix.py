def mirror_matrix(matrix: list[list[int]]) -> list[list[int]]:

    # res = []
    # for mat in matrix:
    #     a = reversed(mat)
    #     res.append(list(a))
    #   #  print (res)
    # return res


    res = matrix[:]
    for mat in res:
        mat.reverse()
    return res

print(mirror_matrix([]))
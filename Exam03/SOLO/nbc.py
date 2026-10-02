def number_base_converter(number: str, from_base: int, to_base: int) -> str:


    if not isinstance(number, str):
        return 'ERROR'
    
    if not(2 <= from_base <= 36 and 2<= to_base <= 36):
        return 'ERROR'

    newbase = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    res = ""
    try:
        nb = int(number, from_base)
    except:
        return 'ERROR'
    if nb == 0:
        res = '0'

    while nb:
        index = nb % to_base
        res = newbase[index] + res
        nb = nb // nb

    return res


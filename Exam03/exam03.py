# exam03.py - Todos los ejercicios del Exam03

# === EASY ===

def echo_validator(text: str) -> bool:
    if not isinstance(text, str):
        return False
    clean = "".join(char.lower() for char in text if char.isalpha())
    if clean == "":
        return False
    return clean == clean[::-1]


def mirror_matrix(matrix: list[list[int]]) -> list[list[int]]:
    return [fila[::-1] for fila in matrix]


def shadow_merge(list1: list[int], list2: list[int]) -> list[int]:
    res = []
    for c in list1:
        res.append(c)
    for c in list2:
        res.append(c)
    res.sort()
    return res


def whisper_cipher(text: str, shift: int) -> str:
    res = ""
    for c in text:
        if c.isalpha():
            if c.islower():
                start = ord('a')
            else:
                start = ord('A')
            newch = chr((ord(c) - start + shift) % 26 + start)
            res += newch
        else:
            res += c
    return res


# === MEDIUM ===

def bracket_validator(s: str) -> bool:
    if not isinstance(s, str):
        return False
    
    stack = []
    pairs = {')': '(', ']': '[', '}': '{'}
    
    for c in s:
        if c in '([{':
            stack.append(c)
        elif c in ')]}':
            if not stack or stack[-1] != pairs[c]:
                return False
            stack.pop()
    
    return len(stack) == 0


def number_base_converter(number: str, from_base: int, to_base: int) -> str:
    if not isinstance(number, str):
        return 'ERROR'
    if not (2 <= from_base <= 36 and 2 <= to_base <= 36):
        return 'ERROR'
    
    newbase = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    res = ""
    
    try:
        nb = int(number, from_base)
    except:
        return 'ERROR'
    if nb == 0:
        return '0'
    
    while nb:
        index = nb % to_base
        res = newbase[index] + res
        nb = nb // to_base
    
    return res


def pattern_tracker(text: str) -> int:
    ctr = 0
    for a, b in zip(text, text[1:]):
        if a.isdigit() and b.isdigit() and int(b) == int(a) + 1:
            ctr += 1
    return ctr


def string_sculptor(text: str) -> str:
    res = ""
    ind = 0
    for char in text:
        if char.isalpha():
            if ind % 2 == 0:
                res += char.lower()
            else:
                res += char.upper()
            ind += 1
        else:
            res += char
    return res


def twist_sequence(arr: list[int], k: int) -> list[int]:
    for i in range(k):
        c = arr.pop()
        arr.insert(0, c)
    return arr


# === HARD ===

def cryptic_sorter(strings: list[str]) -> list[str]:
    def count_vocals(chain: str) -> int:
        vocals = "aeiou"
        return sum(1 for voc in chain.lower() if voc in vocals)
    
    return sorted(strings, key=lambda s: (len(s), s.lower(), count_vocals(s)))


def string_permutation_checker(s1: str, s2: str) -> bool:
    if not isinstance(s1, str) or not isinstance(s2, str):
        return False
    if len(s1) != len(s2):
        return False
    return sorted(s1) == sorted(s2)


# === CONAITOR (referencia) ===

def bracket_validator_ref(s: str) -> bool:
    if not isinstance(s, str):
        return False
    
    stack = []
    pairs = {')': '(', ']': '[', '}': '{'}
    
    for c in s:
        if c in '([{':
            stack.append(c)
        elif c in ')]}':
            if not stack or stack[-1] != pairs[c]:
                return False
            stack.pop()
    
    return len(stack) == 0


def echo_validator_ref(text: str) -> bool:
    if not isinstance(text, str):
        return False
    mia = text.lower().replace(" ", "")
    tuya = mia[::-1]
    return tuya == mia


def mirror_matrix_ref(matrix: list[list[int]]) -> list[list[int]]:
    res = matrix[:]
    for mat in res:
        mat.reverse()
    return res


def number_base_ref(number: str, from_base: int, to_base: int) -> str:
    if not isinstance(number, str):
        return 'ERROR'
    if not (2 <= from_base <= 36 and 2 <= to_base <= 36):
        return 'ERROR'
    
    mybase = "0123456789abcdefghijklmnopqrstuvwxyz"
    res = ""
    
    try:
        nb = int(number, from_base)
    except:
        return 'ERROR'
    
    if nb == 0:
        res += "0"
    
    while nb:
        index = nb % to_base
        res = mybase[index] + res
        nb = nb // to_base
    
    return res


def shadow_merge_ref(list1: list[int], list2: list[int]) -> list[int]:
    res = []
    for c in list1:
        res.append(c)
    for c in list2:
        res.append(c)
    return sorted(res)


def whisper_cipher_ref(text: str, shift: int) -> str:
    minus = "abcdefghijklmnopqrstuvwxyz"
    mayus = minus.upper()
    
    if not isinstance(text, str):
        return False
    if not isinstance(shift, int):
        return False
    res = ""
    
    for c in text:
        if c.isalpha():
            if c in minus:
                index = minus.index(c)
                index += shift
                c = minus[index % 26]
                res += c
            if c in mayus:
                index = mayus.index(c)
                index += shift
                c = mayus[index % 26]
                res += c
        else:
            res += c
    return res


# === TESTS ===

if __name__ == "__main__":
    # EASY
    print("=== EASY ===")
    print("echo_validator:", echo_validator("A man a plan a canal Panama"))
    print("mirror_matrix:", mirror_matrix([[1, 2, 3, 4]]))
    print("shadow_merge:", shadow_merge([1, 2, 7, 5, 0, 4], [2, 3, 4]))
    print("whisper_cipher:", whisper_cipher("xyzabc123", 1))
    
    # MEDIUM
    print("\n=== MEDIUM ===")
    print("bracket_validator:", bracket_validator("()[]{}"))
    print("bracket_validator:", bracket_validator("([{}])"))
    print("bracket_validator:", bracket_validator("(]"))
    print("number_base_converter:", number_base_converter("230", 10, 16))
    print("pattern_tracker:", pattern_tracker("012a34"))
    print("string_sculptor:", string_sculptor("mi MOno AmeLio Y Yo"))
    print("twist_sequence:", twist_sequence([1, 2, 3, 4, 5], 2))
    
    # HARD
    print("\n=== HARD ===")
    print("cryptic_sorter:", cryptic_sorter(["apple", "cat", "banana", "dog", "elephant"]))
    print("string_permutation_checker:", string_permutation_checker("abbc", "bca"))
    
    # CONAITOR (referencia)
    print("\n=== CONAITOR (referencia) ===")
    print("bracket_validator_ref:", bracket_validator_ref("()[]{}"))
    print("echo_validator_ref:", echo_validator_ref("race a ecar"))
    print("mirror_matrix_ref:", mirror_matrix_ref([[1, 2, 3, 4]]))
    print("number_base_ref:", number_base_ref("1010", 2, 10))
    print("shadow_merge_ref:", shadow_merge_ref([], [5, 2, 4, 6]))
    print("whisper_cipher_ref:", whisper_cipher_ref("abc1234xyz", 3))

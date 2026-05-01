def get_values(number):
    # Converts row number to x1, x2, x3 values
    x1 = number // 4
    x2 = (number // 2) % 2
    x3 = number % 2
    return x1, x2, x3


def function_value(x1, x2, x3):
    # Variant 24: not ((x2 or x3) and not (x1 and x3))
    result = not ((x2 or x3) and not (x1 and x3))
    return int(result)


def make_sdnf_element(values):
    # Builds one minterm for SDNF
    names = ["x1", "x2", "x3"]
    parts = []

    for i in range(3):
        if values[i] == 1:
            parts.append(names[i])
        else:
            parts.append("¬" + names[i])

    return "(" + " ∧ ".join(parts) + ")"


def make_sknf_element(values):
    # Builds one maxterm for SKNF
    names = ["x1", "x2", "x3"]
    parts = []

    for i in range(3):
        if values[i] == 0:
            parts.append(names[i])
        else:
            parts.append("¬" + names[i])

    return "(" + " ∨ ".join(parts) + ")"


def main():
    ones = []
    zeros = []
    vector = []

    print("Лабораторная работа №2")
    print("Вариант 24")
    print("Функция: f = ¬((x2 ∨ x3) ∧ ¬(x1 ∧ x3))")
    print()
    print("Таблица истинности:")
    print(" j | x1 x2 x3 | f")
    print("------------------")

    for j in range(8):
        x1, x2, x3 = get_values(j)
        f = function_value(x1, x2, x3)

        vector.append(f)

        if f == 1:
            ones.append(j)
        else:
            zeros.append(j)

        print(f"{j:2} |  {x1}  {x2}  {x3}  | {f}")

    sdnf_parts = []
    for j in ones:
        values = get_values(j)
        sdnf_parts.append(make_sdnf_element(values))

    sknf_parts = []
    for j in zeros:
        values = get_values(j)
        sknf_parts.append(make_sknf_element(values))

    if len(sdnf_parts) > 0:
        sdnf = " ∨ ".join(sdnf_parts)
    else:
        sdnf = "0"

    if len(sknf_parts) > 0:
        sknf = " ∧ ".join(sknf_parts)
    else:
        sknf = "1"

    index = 0
    for j in ones:
        index += 2 ** (7 - j)

    print()
    print("Результаты:")
    print("Вектор значений: f = (" + ",".join(str(v) for v in vector) + ")")
    print("СДНФ: f = " + sdnf)
    print("СКНФ: f = " + sknf)
    print("Числовая форма: f = V(" + ",".join(str(i) for i in ones) + ") = Λ(" + ",".join(str(i) for i in zeros) + ")")
    print("Индекс функции: (i) =", index)


if __name__ == "__main__":
    main()
from itertools import combinations

COUNT_VARS = 3
NAMES = ["x1", "x2", "x3"]


def get_row_values(number):
    # Converts row number to variable values.
    x1 = number // 4
    x2 = (number // 2) % 2
    x3 = number % 2
    return x1, x2, x3


def get_bits(number):
    # Converts row number to a binary string.
    return format(number, "03b")


def source_function(x1, x2, x3):
    # Variant 24 function.
    return int(not ((x2 or x3) and not (x1 and x3)))


def term_to_text(term, form_type):
    # Converts a short binary mask to a logical expression.
    parts = []

    for i in range(COUNT_VARS):
        if term[i] == "-":
            continue

        if form_type == "DNF":
            if term[i] == "1":
                parts.append(NAMES[i])
            else:
                parts.append("¬" + NAMES[i])
        else:
            if term[i] == "0":
                parts.append(NAMES[i])
            else:
                parts.append("¬" + NAMES[i])

    if len(parts) == 0:
        if form_type == "DNF":
            return "1"
        return "0"

    if form_type == "DNF":
        return "(" + " ∧ ".join(parts) + ")"
    return "(" + " ∨ ".join(parts) + ")"


def make_expression(terms, form_type):
    # Joins terms into one formula.
    if len(terms) == 0:
        if form_type == "DNF":
            return "0"
        return "1"

    text_terms = []

    for term in terms:
        text_terms.append(term_to_text(term, form_type))

    if form_type == "DNF":
        return " ∨ ".join(text_terms)
    return " ∧ ".join(text_terms)


def check_perfect_form(terms, form_name):
    # Checks that every term contains all variables.
    correct = True

    for term in terms:
        if len(term) != COUNT_VARS or "-" in term:
            correct = False

    if correct:
        print(form_name + ": соответствует совершенной форме")
    else:
        print(form_name + ": не соответствует совершенной форме")

    return correct


def glue_two(first, second):
    # Returns a glued term if two terms differ in one position.
    diff_count = 0
    diff_place = -1

    for i in range(COUNT_VARS):
        if first[i] != second[i]:
            if first[i] == "-" or second[i] == "-":
                return None

            diff_count += 1
            diff_place = i

    if diff_count != 1:
        return None

    result = list(first)
    result[diff_place] = "-"
    return "".join(result)


def find_prime_terms(start_terms):
    # Performs repeated gluing and returns prime implicants.
    current = sorted(set(start_terms))
    prime_terms = set()
    step = 1

    while True:
        next_terms = set()
        used = set()

        print("Шаг склеивания", step)
        was_gluing = False

        for i in range(len(current)):
            for j in range(i + 1, len(current)):
                glued = glue_two(current[i], current[j])

                if glued is not None:
                    was_gluing = True
                    next_terms.add(glued)
                    used.add(current[i])
                    used.add(current[j])
                    print(" ", current[i], "+", current[j], "->", glued)

        for term in current:
            if term not in used:
                prime_terms.add(term)

        if not was_gluing:
            break

        current = sorted(next_terms)
        step += 1

    return sorted(prime_terms)


def term_covers(term, target):
    # Checks whether a term covers a full binary set.
    for i in range(COUNT_VARS):
        if term[i] != "-" and term[i] != target[i]:
            return False

    return True


def print_cover_table(prime_terms, targets):
    # Prints a coverage table.
    print("Импликанта      | Покрываемые наборы")
    print("----------------|-------------------")

    for term in prime_terms:
        covered = []

        for target in targets:
            if term_covers(term, target):
                covered.append(target)

        print(f"{term:<15} | {', '.join(covered)}")


def remove_extra_terms(prime_terms, targets):
    # Removes terms that do not change the function value.
    result = list(prime_terms)
    changed = True

    while changed:
        changed = False

        for term in result[:]:
            other_terms = []

            for item in result:
                if item != term:
                    other_terms.append(item)

            all_covered = True

            for target in targets:
                covered = False

                for other in other_terms:
                    if term_covers(other, target):
                        covered = True

                if not covered:
                    all_covered = False

            if all_covered:
                print("Лишняя импликанта:", term)
                result.remove(term)
                changed = True
                break

    return sorted(result)


def choose_by_table(prime_terms, targets):
    # Selects the smallest coverage set.
    print_cover_table(prime_terms, targets)

    for size in range(1, len(prime_terms) + 1):
        for group in combinations(prime_terms, size):
            ok = True

            for target in targets:
                found = False

                for term in group:
                    if term_covers(term, target):
                        found = True

                if not found:
                    ok = False

            if ok:
                return sorted(list(group))

    return []


def calculation_method(targets, form_type):
    # Calculation minimization method.
    prime_terms = find_prime_terms(targets)
    print("Сокращенная форма:", make_expression(prime_terms, form_type))

    result = remove_extra_terms(prime_terms, targets)
    return result


def quine_method(targets, form_type):
    # Quine-McCluskey minimization method.
    prime_terms = find_prime_terms(targets)
    print("Сокращенная форма:", make_expression(prime_terms, form_type))

    result = choose_by_table(prime_terms, targets)
    return result


def print_karnaugh(values):
    # Prints a Karnaugh map for three variables.
    gray_cols = [0, 1, 3, 2]

    print("Карта Карно")
    print("        x2x3: 00  01  11  10")
    print("              ----------------")

    for x1 in [0, 1]:
        row = []

        for col in gray_cols:
            number = x1 * 4 + col
            row.append(str(values[number]))

        print("x1 =", x1, "       ", "   ".join(row))


def karnaugh_method(targets, form_type, values):
    # Karnaugh method for three variables.
    print_karnaugh(values)

    print("Группировка соседних клеток карты Карно:")

    prime_terms = find_prime_terms_silent(targets)

    for term in prime_terms:
        covered = []

        for target in targets:
            if term_covers(term, target):
                covered.append(target)

        print(" ", term, "->", ", ".join(covered))

    result = remove_extra_terms(prime_terms, targets)

    return result

def find_prime_terms_silent(start_terms):
    # Finds prime implicants without printing gluing steps.
    current = sorted(set(start_terms))
    prime_terms = set()

    while True:
        next_terms = set()
        used = set()
        was_gluing = False

        for i in range(len(current)):
            for j in range(i + 1, len(current)):
                glued = glue_two(current[i], current[j])

                if glued is not None:
                    was_gluing = True
                    next_terms.add(glued)
                    used.add(current[i])
                    used.add(current[j])

        for term in current:
            if term not in used:
                prime_terms.add(term)

        if not was_gluing:
            break

        current = sorted(next_terms)

    return sorted(prime_terms)

def vector_by_terms(terms, form_type):
    # Builds a truth vector from minimized DNF or CNF terms.
    result = []

    for number in range(8):
        bits = get_bits(number)

        if form_type == "DNF":
            value = 0

            for term in terms:
                if term_covers(term, bits):
                    value = 1

            result.append(value)
        else:
            value = 1

            for term in terms:
                if term_covers(term, bits):
                    value = 0

            result.append(value)

    return result


def make_full_dnf_from_vector(values):
    # Converts a truth vector to SDNF.
    terms = []

    for number in range(8):
        if values[number] == 1:
            terms.append(get_bits(number))

    return make_expression(terms, "DNF")


def make_full_cnf_from_vector(values):
    # Converts a truth vector to SKNF.
    terms = []

    for number in range(8):
        if values[number] == 0:
            terms.append(get_bits(number))

    return make_expression(terms, "CNF")


def main():
    values = []
    ones = []
    zeros = []

    print("Лабораторная работа №3")
    print("Вариант 24")
    print("f = ¬((x2 ∨ x3) ∧ ¬(x1 ∧ x3))")
    print()

    print("Таблица истинности")
    print(" j | x1 x2 x3 | f")
    print("------------------")

    for number in range(8):
        x1, x2, x3 = get_row_values(number)
        value = source_function(x1, x2, x3)
        bits = get_bits(number)

        values.append(value)

        if value == 1:
            ones.append(bits)
        else:
            zeros.append(bits)

        print(f"{number:2} |  {x1}  {x2}  {x3}  | {value}")

    print()
    print("Вектор значений:", values)
    print("СДНФ: f =", make_expression(ones, "DNF"))
    print("СКНФ: f =", make_expression(zeros, "CNF"))
    print()

    print("Проверка исходных форм")
    check_perfect_form(ones, "СДНФ")
    check_perfect_form(zeros, "СКНФ")
    print()

    print("1. Расчетный метод для СДНФ")
    calc_dnf = calculation_method(ones, "DNF")
    print("ТДНФ: f =", make_expression(calc_dnf, "DNF"))
    print()

    print("1. Расчетный метод для СКНФ")
    calc_cnf = calculation_method(zeros, "CNF")
    print("ТКНФ: f =", make_expression(calc_cnf, "CNF"))
    print()

    print("2. Метод Квайна-Мак-Класки для СДНФ")
    quine_dnf = quine_method(ones, "DNF")
    print("ТДНФ: f =", make_expression(quine_dnf, "DNF"))
    print()

    print("2. Метод Квайна-Мак-Класки для СКНФ")
    quine_cnf = quine_method(zeros, "CNF")
    print("ТКНФ: f =", make_expression(quine_cnf, "CNF"))
    print()

    print("3. Табличный метод Карно для СДНФ")
    karno_dnf = karnaugh_method(ones, "DNF", values)
    print("ТДНФ: f =", make_expression(karno_dnf, "DNF"))
    print()

    print("3. Табличный метод Карно для СКНФ")
    karno_cnf = karnaugh_method(zeros, "CNF", values)
    print("ТКНФ: f =", make_expression(karno_cnf, "CNF"))
    print()

    print("Сравнение трех методов")
    dnf_equal = calc_dnf == quine_dnf == karno_dnf
    cnf_equal = calc_cnf == quine_cnf == karno_cnf

    print("ТДНФ совпадают:", "да" if dnf_equal else "нет")
    print("ТКНФ совпадают:", "да" if cnf_equal else "нет")
    print()

    dnf_vector = vector_by_terms(calc_dnf, "DNF")
    cnf_vector = vector_by_terms(calc_cnf, "CNF")

    print("Преобразование для сравнения")
    print("ТДНФ -> СКНФ: f =", make_full_cnf_from_vector(dnf_vector))
    print("ТКНФ -> СДНФ: f =", make_full_dnf_from_vector(cnf_vector))
    print("ТДНФ и ТКНФ задают одну функцию:", "да" if dnf_vector == cnf_vector else "нет")
    print()

    print("Итог")
    print("ТДНФ: f =", make_expression(calc_dnf, "DNF"))
    print("ТКНФ: f =", make_expression(calc_cnf, "CNF"))


if __name__ == "__main__":
    main()
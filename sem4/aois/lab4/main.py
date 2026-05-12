import itertools
from tabulate import tabulate


def to_bin(val, num_vars=3):
    return format(val, f'0{num_vars}b')


def format_term(bin_str, mode='DNF', vars_list=None):
    if vars_list is None: vars_list = ["x1", "x2", "x3", "x4"]
    parts = []
    for i, bit in enumerate(bin_str):
        if bit == '-': continue
        var = vars_list[i]
        if mode == 'DNF':
            parts.append(var if bit == '1' else f"!{var}")
        else:
            parts.append(var if bit == '0' else f"!{var}")
    if not parts: return "1" if mode == 'DNF' else "0"
    sep = " * " if mode == 'DNF' else " + "
    return "(" + sep.join(parts) + ")"


def get_diff(s1, s2):
    diff_count = 0
    idx = -1
    for i in range(len(s1)):
        if s1[i] != s2[i]:
            diff_count += 1
            idx = i
    return idx if diff_count == 1 else -1


def gluing_step(terms, dont_cares=None):
    all_terms = set(terms)
    if dont_cares:
        all_terms.update(dont_cares)

    current_terms = all_terms
    prime_implicants = set()

    while True:
        new_terms = set()
        used = set()
        t_list = list(current_terms)
        for i in range(len(t_list)):
            for j in range(i + 1, len(t_list)):
                idx = get_diff(t_list[i], t_list[j])
                if idx != -1:
                    res = list(t_list[i])
                    res[idx] = '-'
                    new_terms.add("".join(res))
                    used.add(t_list[i])
                    used.add(t_list[j])

        for t in t_list:
            if t not in used and any(c != '-' for c in t):
                prime_implicants.add(t)
        if not new_terms: break
        current_terms = new_terms
    return list(prime_implicants)


def coverage_step(implicants, targets):
    if not targets: return []
    table = {imp: [t for t in targets if all(imp[i] == t[i] or imp[i] == '-' for i in range(len(t)))] for imp in
             implicants}

    uncovered = set(targets)
    tupik_form = []
    while uncovered:
        best_imp = max(implicants, key=lambda imp: len(set(table[imp]) & uncovered))
        tupik_form.append(best_imp)
        uncovered -= set(table[best_imp])
    return tupik_form


def run_task_1():
    print("\n=== ЗАДАНИЕ 1: ОДВ-3 (Вычитатель: СДНФ) ===")
    headers = ["№", "a", "b", "Pin", "D", "Pout"]
    table, d_ones, p_ones = [], [], []

    for i in range(8):
        a, b, pin = (i >> 2) & 1, (i >> 1) & 1, i & 1

        res = a - b - pin

        d = res % 2

        pout = 1 if res < 0 else 0

        table.append([i, a, b, pin, d, pout])

        if d == 1: d_ones.append(to_bin(i, 3))
        if pout == 1: p_ones.append(to_bin(i, 3))

    print(tabulate(table, headers=headers, tablefmt="grid"))

    v = ["a", "b", "Pin"]
    for name, ones in [("D", d_ones), ("Pout", p_ones)]:
        primes = gluing_step(ones)
        tupik = coverage_step(primes, ones)
        final_form = " + ".join([format_term(imp, 'DNF', v) for imp in tupik])
        print(f"Минимизированная СДНФ {name}: {final_form}")


def run_task_2():
    print("\n=== ЗАДАНИЕ 2: Преобразователь 8421 -> 8421+2 ===")
    headers = ["№", "x1", "x2", "x3", "x4", "y1", "y2", "y3", "y4"]
    table = []
    outputs_ones = [[] for _ in range(4)]
    dont_cares = [to_bin(i, 4) for i in range(10, 16)]

    for i in range(16):
        x_bits = to_bin(i, 4)
        if i <= 9:
            y_val = (i + 2) % 16
            y_bits = to_bin(y_val, 4)
            for j in range(4):
                if y_bits[j] == '1': outputs_ones[j].append(x_bits)
            table.append([i] + list(x_bits) + list(y_bits))
        else:
            table.append([i] + list(x_bits) + ["-"] * 4)

    print(tabulate(table, headers=headers, tablefmt="grid"))

    v = ["x1", "x2", "x3", "x4"]
    for i in range(4):
        primes = gluing_step(outputs_ones[i], dont_cares)
        tupik = coverage_step(primes, outputs_ones[i])
        final_form = " + ".join([format_term(imp, 'DNF', v) for imp in tupik])
        print(f"Минимизированная y{i + 1}: {final_form}")


if __name__ == "__main__":
    run_task_1()
    run_task_2()
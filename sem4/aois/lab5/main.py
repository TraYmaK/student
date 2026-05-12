import itertools
from tabulate import tabulate
 
def to_bin(val, num_vars=4):
    """Перевод числа в двоичную строку заданной длины."""
    return format(val, f'0{num_vars}b')
 
def format_term(bin_str, mode='DNF', vars_list=None):
    """Преобразует двоичную строку вида '1-0' в алгебраический вид."""
    if vars_list is None: vars_list = ["q3*", "q2*", "q1*", "V"]
    parts = []
    for i, bit in enumerate(bin_str):
        if bit == '-': continue
        var = vars_list[i]
        if mode == 'DNF':
            parts.append(var if bit == '1' else f"!{var}")
        else:
            parts.append(var if bit == '0' else f"!{var}")
   
    if not parts: return "1"
    sep = " * " if mode == 'DNF' else " + "
    return "(" + sep.join(parts) + ")"
 
def get_diff(s1, s2):
    """Ищет индекс единственного различающегося бита для склеивания."""
    diff_count = 0
    idx = -1
    for i in range(len(s1)):
        if s1[i] != s2[i]:
            diff_count += 1
            idx = i
    return idx if diff_count == 1 else -1
 
def gluing_step(terms):
    """Этап склеивания соседних конституэнт."""
    current_terms = set(terms)
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
            if t not in used:
                prime_implicants.add(t)
       
        if not new_terms: break
        current_terms = new_terms
    return list(prime_implicants)
 
def coverage_step(implicants, targets):
    """Поиск минимального покрытия (метод Квайна-Мак-Класки)."""
    if not targets: return []
    table = {imp: [t for t in targets if all(imp[i] == t[i] or imp[i] == '-' for i in range(len(t)))] for imp in implicants}
   
    uncovered = set(targets)
    tupik_form = []
    while uncovered:
        best_imp = max(implicants, key=lambda imp: len(set(table[imp]) & uncovered))
        tupik_form.append(best_imp)
        uncovered -= set(table[best_imp])
    return tupik_form
 
def main():
    print("=== ЛАБОРАТОРНАЯ РАБОТА №5: СИНТЕЗ ЦИФРОВЫХ АВТОМАТОВ ===")
    print("Вариант 02: Вычитающий счетчик на 8 состояний (Т-триггер)\n")
 
    headers = ["№", "q3*", "q2*", "q1*", "V", "q3", "q2", "q1", "h3", "h2", "h1"]
    table_data = []
   
    h_ones = [[] for _ in range(3)]
   
    for i in range(16):
        #
        state_val = i >> 1
        v = i & 1
       
        q_star = to_bin(state_val, 3)
       
        if v == 1:
            next_state_val = (state_val - 1) % 8
        else:
            next_state_val = state_val
           
        q_next = to_bin(next_state_val, 3)
       
        h = [int(q_star[j] != q_next[j]) for j in range(3)]
       
        input_set = q_star + str(v)
        for idx in range(3):
            if h[idx] == 1:
                h_ones[idx].append(input_set)
       
        table_data.append([i, q_star[0], q_star[1], q_star[2], v,
                           q_next[0], q_next[1], q_next[2],
                           h[0], h[1], h[2]])
 
    print(tabulate(table_data, headers=headers, tablefmt="grid"))
 
    print("\n>>> РЕЗУЛЬТАТЫ МИНИМИЗАЦИИ ФУНКЦИЙ ВОЗБУЖДЕНИЯ <<<")
   
    v_names = ["q3*", "q2*", "q1*", "V"]
    for i in range(3):
        h_idx = 3 - i
        print(f"\n--- Функция h{h_idx} ---")
       
        if not h_ones[i]:
            print(f"h{h_idx} = 0")
            continue
           
        primes = gluing_step(h_ones[i])
        tupik = coverage_step(primes, h_ones[i])
       
        final_formula = " ∨ ".join([format_term(imp, 'DNF', v_names) for imp in tupik])
        print(f"h{h_idx} = {final_formula}")
 
if __name__ == "__main__":
    main()
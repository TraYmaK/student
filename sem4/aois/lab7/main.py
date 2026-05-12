import random


class AssociativeProcessor:
    def __init__(self, m=10, n=8):
        self.m = m  # Количество слов
        self.n = n  # Количество разрядов
        # Генерация случайного массива данных (память АЗУ)
        self.memory = [[random.randint(0, 1) for _ in range(n)] for _ in range(m)]

    def bits_to_int(self, bits):
        """Вспомогательная функция для перевода из двоичной системы в число"""
        return int("".join(map(str, bits)), 2)

    def int_to_bits(self, value):
        """Вспомогательная функция для перевода числа в двоичный список"""
        return [int(b) for b in format(value, f'0{self.n}b')]

    def compare_recurrence(self, word, argument):
        """
        Реализация рекуррентных формул сравнения из методички.
        Возвращает (g, l):
        (0, 0) -> Равно
        (1, 0) -> Больше
        (0, 1) -> Меньше
        """
        g_prev = 0
        l_prev = 0

        # Сравнение идет от старшего разряда (индекс 0) к младшему
        for i in range(self.n):
            a_i = argument[i]
            s_ji = word[i]

            # Отрицания
            not_a_i = 1 if a_i == 0 else 0
            not_s_ji = 1 if s_ji == 0 else 0
            not_g_prev = 1 if g_prev == 0 else 0
            not_l_prev = 1 if l_prev == 0 else 0

            # Формулы из методички
            g_curr = g_prev | (not_a_i & s_ji & not_l_prev)
            l_curr = l_prev | (a_i & not_s_ji & not_g_prev)

            g_prev, l_prev = g_curr, l_curr

        return g_prev, l_prev

    def search_in_interval(self, min_val, max_val):
        """Поиск величин, входящих в интервал (min_val, max_val)"""
        arg_min = self.int_to_bits(min_val)
        arg_max = self.int_to_bits(max_val)

        results = []

        print(f"\nВыполняется поиск в интервале: {min_val} < S_j < {max_val}")
        print("-" * 65)
        print(f"{'№':<3} | {'Слово (Bin)':<10} | {'Dec':<4} | {'S > Min':<7} | {'S < Max':<7} | {'Итог'}")
        print("-" * 65)

        for j in range(self.m):
            word = self.memory[j]
            val = self.bits_to_int(word)

            # Сравнение с нижней границей (должно быть S > min)
            g_min, l_min = self.compare_recurrence(word, arg_min)
            is_greater_than_min = (g_min == 1 and l_min == 0)

            # Сравнение с верхней границей (должно быть S < max)
            g_max, l_max = self.compare_recurrence(word, arg_max)
            is_less_than_max = (g_max == 0 and l_max == 1)

            status = "ПОДХОДИТ" if (is_greater_than_min and is_less_than_max) else "-"

            bin_str = "".join(map(str, word))
            print(
                f"{j:<3} | {bin_str:<10} | {val:<4} | {str(is_greater_than_min):<7} | {str(is_less_than_max):<7} | {status}")

            if status == "ПОДХОДИТ":
                results.append(val)

        return results


# --- Запуск ---
if __name__ == "__main__":
    # Создаем процессор: 15 слов по 8 бит
    processor = AssociativeProcessor(m=15, n=8)

    # Задаем интервал для поиска
    low = 50
    high = 150

    found = processor.search_in_interval(low, high)

    print("-" * 65)
    print(f"Найдено подходящих чисел: {len(found)}")
    print(f"Список: {found}")
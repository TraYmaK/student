class HashTablePhysics:
    def __init__(self, size=20):
        self.H = size  # Размер таблицы
        self.B = 0  # Начальный адрес
        # Структура строки ТХ: [ID, C, U, T, L, D, Po, Pi_Data]
        # C-коллизия, U-занято, T-терминальный, L-связь, D-удаление, Po-указатель
        self.table = [["", 0, 0, 0, 0, 0, None, ""] for _ in range(self.H)]
        self.alphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"

    def calculate_v(self, word):
        """Расчет числового значения V по первым двум буквам"""
        word = word.upper()
        # Если слово короткое, берем 0 для второй буквы
        idx1 = self.alphabet.find(word[0])
        idx2 = self.alphabet.find(word[1]) if len(word) > 1 else 0

        if idx1 == -1: idx1 = 0  # Защита от символов вне алфавита
        if idx2 == -1: idx2 = 0

        v = idx1 * 33 + idx2
        return v

    def get_hash(self, v):
        """Расчет хеш-адреса h = V mod H + B"""
        return (v % self.H) + self.B

    def add_record(self, word, description):
        """Добавление записи с обработкой коллизий (внутренняя адресация)"""
        v = self.calculate_v(word)
        h = self.get_hash(v)

        # 1. Если ячейка по хеш-адресу свободна
        if self.table[h][2] == 0:
            self.table[h][0] = word
            self.table[h][2] = 1  # U = 1 (занято)
            self.table[h][3] = 1  # T = 1 (терминальный, т.к. один в цепи)
            self.table[h][7] = description
            return h

        # 2. Если возникла коллизия
        else:
            self.table[h][1] = 1  # С = 1 (признак коллизии)

            # Ищем последнюю запись в текущей цепочке коллизий
            current_idx = h
            while self.table[current_idx][6] is not None:
                current_idx = self.table[current_idx][6]

            # Ищем свободную ячейку в таблице для резерва (линейный поиск с конца)
            free_idx = -1
            for i in range(self.H - 1, -1, -1):
                if self.table[i][2] == 0:
                    free_idx = i
                    break

            if free_idx != -1:
                # Обновляем предыдущую "последнюю" запись
                self.table[current_idx][3] = 0  # Больше не терминальная
                self.table[current_idx][6] = free_idx  # Po указывает на новую

                # Заполняем новую ячейку
                self.table[free_idx][0] = word
                self.table[free_idx][2] = 1
                self.table[free_idx][3] = 1  # Теперь это терминальная
                self.table[free_idx][7] = description
                return free_idx
            else:
                print("Таблица переполнена!")
                return None

    def delete_record(self, word):
        """Удаление записи по алгоритму из методички (установка флага D)"""
        v = self.calculate_v(word)
        h = self.get_hash(v)

        # Поиск слова в цепочке
        curr = h
        while curr is not None:
            if self.table[curr][0].upper() == word.upper():
                self.table[curr][5] = 1  # Устанавливаем флаг D = 1
                print(f"Запись '{word}' помечена на удаление (D=1).")
                return
            curr = self.table[curr][6]
        print(f"Слово '{word}' не найдено.")

    def print_table(self):
        """Вывод таблицы в консоль"""
        print("-" * 95)
        print(
            f"{'№':<3} | {'ID (Ключ)':<12} | {'C':<2} | {'U':<2} | {'T':<2} | {'L':<2} | {'D':<2} | {'Po':<3} | {'Данные (Pi)':<30}")
        print("-" * 95)
        for i in range(self.H):
            row = self.table[i]
            po_val = row[6] if row[6] is not None else ""
            print(
                f"{i:<3} | {row[0]:<12} | {row[1]:<2} | {row[2]:<2} | {row[3]:<2} | {row[4]:<2} | {row[5]:<2} | {po_val:<3} | {row[7]:<30}")
        print("-" * 95)


# --- ОСНОВНОЙ БЛОК ПРОГРАММЫ ---
if __name__ == "__main__":
    ht = HashTablePhysics()

    # Исходные данные по физике (Вариант 2)
    # Подобраны так, чтобы были коллизии (Атом и Гармоника имеют один h)
    physics_data = [
        ("Атом", "Частица вещества"),
        ("Бозон", "Элементарная частица"),
        ("Ватт", "Единица мощности"),
        ("Вольт", "Единица напряжения"),
        ("Герц", "Единица частоты"),
        ("Гармоника", "Колебание частоты"),
        ("Диод", "Полупроводник"),
        ("Джоуль", "Единица энергии"),
        ("Заряд", "Свойство тел"),
        ("Изотоп", "Вид атомов"),
        ("Кварк", "Частица адрона"),
        ("Линза", "Оптический прибор")
    ]

    print("Шаг 1: Формирование хеш-таблицы...")
    for word, desc in physics_data:
        ht.add_record(word, desc)

    ht.print_table()

    print("\nШаг 2: Демонстрация удаления (например, 'Ватт')...")
    ht.delete_record("Ватт")

    ht.print_table()

    # Вывод расчетных значений для первых двух слов для отчета
    print("\nПримеры расчетов V и h:")
    for word, _ in physics_data[:3]:
        v = ht.calculate_v(word)
        h = ht.get_hash(v)
        print(f"Слово: {word}, V = {v}, h = {h}")

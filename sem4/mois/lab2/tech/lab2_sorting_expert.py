from experta import Fact, KnowledgeEngine, MATCH, NOT, Rule, watch


class SortingProperty(Fact):
    pass


class AlgorithmType(Fact):
    pass


class ReportedResult(Fact):
    pass


class SortingClassifier(KnowledgeEngine):
    def __init__(self):
        super().__init__()
        self.final_result: str | None = None

    def _trace(self, rule_name: str, message: str) -> None:
        print(f"[RULE] {rule_name}: {message}")

    @Rule(
        SortingProperty(name="stable", value="yes"),
        SortingProperty(name="extra_memory", value="o_n"),
        SortingProperty(name="best_case", value="n_log_n"),
        SortingProperty(name="worst_case", value="n_log_n"),
        NOT(AlgorithmType(name="merge_sort")),
    )
    def detect_merge_sort(self):
        self._trace("detect_merge_sort", "Профиль соответствует сортировке слиянием")
        self.declare(AlgorithmType(name="merge_sort"))

    @Rule(
        SortingProperty(name="stable", value="no"),
        SortingProperty(name="extra_memory", value="o_1"),
        SortingProperty(name="best_case", value="n_log_n"),
        SortingProperty(name="worst_case", value="n_log_n"),
        NOT(AlgorithmType(name="heap_sort")),
    )
    def detect_heap_sort(self):
        self._trace("detect_heap_sort", "Профиль соответствует пирамидальной сортировке")
        self.declare(AlgorithmType(name="heap_sort"))

    @Rule(
        SortingProperty(name="stable", value="no"),
        SortingProperty(name="extra_memory", value="o_log_n"),
        SortingProperty(name="best_case", value="n_log_n"),
        SortingProperty(name="worst_case", value="n2"),
        NOT(AlgorithmType(name="quick_sort")),
    )
    def detect_quick_sort(self):
        self._trace("detect_quick_sort", "Профиль соответствует быстрой сортировке")
        self.declare(AlgorithmType(name="quick_sort"))

    @Rule(
        SortingProperty(name="stable", value="yes"),
        SortingProperty(name="extra_memory", value="o_n_plus_k"),
        SortingProperty(name="best_case", value="n_plus_k"),
        SortingProperty(name="worst_case", value="n_plus_k"),
        NOT(AlgorithmType(name="counting_sort")),
    )
    def detect_counting_sort(self):
        self._trace("detect_counting_sort", "Профиль соответствует сортировке подсчетом")
        self.declare(AlgorithmType(name="counting_sort"))

    @Rule(
        SortingProperty(name="stable", value="no"),
        SortingProperty(name="extra_memory", value="o_1"),
        SortingProperty(name="best_case", value="n2"),
        SortingProperty(name="worst_case", value="n2"),
        NOT(AlgorithmType(name="selection_sort")),
    )
    def detect_selection_sort(self):
        self._trace("detect_selection_sort", "Профиль соответствует сортировке выбором")
        self.declare(AlgorithmType(name="selection_sort"))

    @Rule(
        SortingProperty(name="stable", value="yes"),
        SortingProperty(name="extra_memory", value="o_1"),
        SortingProperty(name="best_case", value="n"),
        SortingProperty(name="worst_case", value="n2"),
        NOT(AlgorithmType(name="ambiguous_quadratic_stable")),
    )
    def detect_ambiguous_quadratic_stable(self):
        self._trace(
            "detect_ambiguous_quadratic_stable",
            "Возможна сортировка вставками или оптимизированная пузырьковая сортировка",
        )
        self.declare(AlgorithmType(name="ambiguous_quadratic_stable"))

    @Rule(
        SortingProperty(name=MATCH.name, value=MATCH.value),
        NOT(AlgorithmType()),
        salience=-10,
    )
    def fallback_unknown(self, name, value):
        self._trace("fallback_unknown", f"Нет точного совпадения для признака {name}={value}")
        self.declare(AlgorithmType(name="unknown"))

    @Rule(
        AlgorithmType(name=MATCH.name),
        NOT(ReportedResult()),
        salience=-20,
    )
    def print_result(self, name):
        self.final_result = name
        print(f"[RESULT] Тип алгоритма: {name}")
        self.declare(ReportedResult())


INPUT_OPTIONS = {
    "stable": ("yes", "no"),
    "extra_memory": ("o_1", "o_log_n", "o_n", "o_n_plus_k"),
    "best_case": ("n", "n_log_n", "n2", "n_plus_k"),
    "worst_case": ("n_log_n", "n2", "n_plus_k"),
}

QUESTION_TEXT = {
    "stable": {
        "title": "1) Устойчивость сортировки",
        "description": (
            "Сохраняет ли алгоритм относительный порядок элементов с одинаковым ключом?"
        ),
        "options": {
            "yes": "да, порядок равных элементов сохраняется",
            "no": "нет, порядок равных элементов может меняться",
        },
        "aliases": {"да": "yes", "нет": "no"},
    },
    "extra_memory": {
        "title": "2) Использование дополнительной памяти",
        "description": (
            "Оцените объём вспомогательной памяти, не считая входной массив."
        ),
        "options": {
            "o_1": "константная память O(1) (in-place)",
            "o_log_n": "логарифмическая память O(log n) (например, стек рекурсии)",
            "o_n": "линейная память O(n)",
            "o_n_plus_k": "память O(n + k)",
        },
        "aliases": {},
    },
    "best_case": {
        "title": "3) Сложность в лучшем случае",
        "description": "Укажите асимптотику времени для наиболее благоприятного входа.",
        "options": {
            "n": "O(n)",
            "n_log_n": "O(n log n)",
            "n2": "O(n^2)",
            "n_plus_k": "O(n + k)",
        },
        "aliases": {},
    },
    "worst_case": {
        "title": "4) Сложность в худшем случае",
        "description": "Укажите асимптотику времени для наименее благоприятного входа.",
        "options": {
            "n_log_n": "O(n log n)",
            "n2": "O(n^2)",
            "n_plus_k": "O(n + k)",
        },
        "aliases": {},
    },
}

ALGORITHM_LABELS = {
    "merge_sort": "Сортировка слиянием (Merge Sort)",
    "heap_sort": "Пирамидальная сортировка (Heap Sort)",
    "quick_sort": "Быстрая сортировка (Quick Sort)",
    "counting_sort": "Сортировка подсчётом (Counting Sort)",
    "selection_sort": "Сортировка выбором (Selection Sort)",
    "ambiguous_quadratic_stable": (
        "Неоднозначно: сортировка вставками или оптимизированная пузырьковая"
    ),
    "unknown": "Не удалось определить алгоритм по текущим признакам",
}


def classify_from_facts(facts: list[tuple[str, str]]) -> str:
    engine = SortingClassifier()
    engine.reset()
    for name, value in facts:
        engine.declare(SortingProperty(name=name, value=value))
    engine.run()
    return engine.final_result or "unknown"


def ask_value(property_name: str) -> str:
    question = QUESTION_TEXT[property_name]
    allowed = INPUT_OPTIONS[property_name]
    aliases = question["aliases"]
    indexed_options = {str(index): code for index, code in enumerate(allowed, start=1)}
    print(f"\n{question['title']}")
    print(question["description"])
    print("Варианты ответа (введите номер):")
    for index, code in indexed_options.items():
        print(f"  {index}) {question['options'][code]} [{code}]")
    while True:
        value = input("Ваш выбор: ").strip().lower()
        if value in indexed_options:
            return indexed_options[value]
        value = aliases.get(value, value)
        if value in allowed:
            return value
        print("Некорректный ввод. Введите номер варианта из списка.")


def run_interactive() -> None:
    print("Классификатор алгоритмов сортировки по профилю поведения")
    print("Введите параметры своего варианта из лабораторного задания.")
    print("Допустимые значения указаны рядом с каждым параметром.")
    facts = [
        ("stable", ask_value("stable")),
        ("extra_memory", ask_value("extra_memory")),
        ("best_case", ask_value("best_case")),
        ("worst_case", ask_value("worst_case")),
    ]
    print("\nИсходные факты:")
    for name, value in facts:
        print(f"  - {name} = {value}")
    result = classify_from_facts(facts)
    print(f"\nИтоговая классификация: {ALGORITHM_LABELS.get(result, result)}")


if __name__ == "__main__":
    enable_trace = input("Включить пошаговую трассировку правил? (y/n): ").strip().lower()
    if enable_trace.startswith("y"):
        watch("RULES", "FACTS")
    run_interactive()

from itertools import product

from tech.lab2_sorting_expert import INPUT_OPTIONS, classify_from_facts


def _facts(stable: str, extra_memory: str, best_case: str, worst_case: str):
    return [
        ("stable", stable),
        ("extra_memory", extra_memory),
        ("best_case", best_case),
        ("worst_case", worst_case),
    ]


def _expected_algorithm(stable: str, extra_memory: str, best_case: str, worst_case: str) -> str:
    if (
        stable == "yes"
        and extra_memory == "o_n"
        and best_case == "n_log_n"
        and worst_case == "n_log_n"
    ):
        return "merge_sort"

    if (
        stable == "no"
        and extra_memory == "o_1"
        and best_case == "n_log_n"
        and worst_case == "n_log_n"
    ):
        return "heap_sort"

    if (
        stable == "no"
        and extra_memory == "o_log_n"
        and best_case == "n_log_n"
        and worst_case == "n2"
    ):
        return "quick_sort"

    if (
        stable == "yes"
        and extra_memory == "o_n_plus_k"
        and best_case == "n_plus_k"
        and worst_case == "n_plus_k"
    ):
        return "counting_sort"

    if stable == "no" and extra_memory == "o_1" and best_case == "n2" and worst_case == "n2":
        return "selection_sort"

    if stable == "yes" and extra_memory == "o_1" and best_case == "n" and worst_case == "n2":
        return "ambiguous_quadratic_stable"

    return "unknown"


def test_detects_each_supported_algorithm_profile():
    known_profiles = [
        (_facts("yes", "o_n", "n_log_n", "n_log_n"), "merge_sort"),
        (_facts("no", "o_1", "n_log_n", "n_log_n"), "heap_sort"),
        (_facts("no", "o_log_n", "n_log_n", "n2"), "quick_sort"),
        (_facts("yes", "o_n_plus_k", "n_plus_k", "n_plus_k"), "counting_sort"),
        (_facts("no", "o_1", "n2", "n2"), "selection_sort"),
        (_facts("yes", "o_1", "n", "n2"), "ambiguous_quadratic_stable"),
    ]

    for facts, expected in known_profiles:
        assert classify_from_facts(facts) == expected


def test_exhaustive_all_input_combinations_are_classified_consistently():
    all_stable = INPUT_OPTIONS["stable"]
    all_extra_memory = INPUT_OPTIONS["extra_memory"]
    all_best_case = INPUT_OPTIONS["best_case"]
    all_worst_case = INPUT_OPTIONS["worst_case"]

    for stable, extra_memory, best_case, worst_case in product(
        all_stable, all_extra_memory, all_best_case, all_worst_case
    ):
        facts = _facts(stable, extra_memory, best_case, worst_case)
        expected = _expected_algorithm(stable, extra_memory, best_case, worst_case)
        assert classify_from_facts(facts) == expected

#include <iostream>
#include <string>
#include <vector>

#define LOIS_LAB1_TEST
#include "main.cpp"

using namespace std;

struct TestState {
    int passed = 0;
    int failed = 0;
};

void expect_true(const bool value, const string& name, TestState& state) {
    if (value) {
        ++state.passed;
    } else {
        ++state.failed;
        cout << "FAIL: " << name << '\n';
    }
}

void expect_false(const bool value, const string& name, TestState& state) {
    expect_true(!value, name, state);
}

void expect_eq_int(const int actual, const int expected, const string& name, TestState& state) {
    if (actual == expected) {
        ++state.passed;
    } else {
        ++state.failed;
        cout << "FAIL: " << name << " (actual=" << actual << ", expected=" << expected << ")\n";
    }
}

void expect_eq_str(const string& actual, const string& expected, const string& name, TestState& state) {
    if (actual == expected) {
        ++state.passed;
    } else {
        ++state.failed;
        cout << "FAIL: " << name << " (actual=\"" << actual << "\", expected=\"" << expected << "\")\n";
    }
}

string validate_like_main(string formula) {
    formula = remove_spaces(formula);

    if (formula.empty()) {
        return "Ошибка: Заполните поле для работы программы!";
    }

    if (!is_valid_alphabet(formula)) {
        return "Ошибка: Недопустимые символы в формуле!";
    }

    if (!(formula.size() == 1 && is_variable(formula[0]))) {
        if (!repeat_variables(formula)) {
            return "Ошибка: Недопустимы подряд идущие переменные (например, AB)!";
        }

        if (!(formula.front() == '(' && formula.back() == ')')) {
            return "Ошибка: Формула должна быть заключена в скобки!";
        }

        if (!has_valid_operators(formula)) {
            return "Ошибка: Неверное использование операторов /, \\, ~ или ->!";
        }

        if (!has_single_operator(formula)) {
            return "Ошибка: Допустима только одна бинарная операция!";
        }

        if (!has_balanced_brackets(formula)) {
            return "Ошибка: Несбалансированы скобки!";
        }

        if (!has_valid_negations(formula)) {
            return "Ошибка: Некорректное отрицание!";
        }
    }

    return "Количество различных подформул: " + to_string(count_subformulas(formula));
}

void test_is_variable(TestState& state) {
    expect_true(is_variable('A'), "is_variable A", state);
    expect_true(is_variable('Z'), "is_variable Z", state);
    expect_true(is_variable('0'), "is_variable 0", state);
    expect_true(is_variable('1'), "is_variable 1", state);
    expect_false(is_variable('a'), "is_variable a", state);
    expect_false(is_variable('#'), "is_variable #", state);
}

void test_is_valid_alphabet(TestState& state) {
    expect_true(is_valid_alphabet("((!A)/\\(B\\/C))"), "valid alphabet complex", state);
    expect_true(is_valid_alphabet("(A->B)"), "valid alphabet implication symbols", state);
    expect_false(is_valid_alphabet("(A@B)"), "invalid alphabet @", state);
    expect_false(is_valid_alphabet("(A-B)"), "invalid alphabet bare minus", state);
}

void test_has_balanced_brackets(TestState& state) {
    expect_true(has_balanced_brackets("(A/\\B)"), "balanced simple", state);
    expect_true(has_balanced_brackets("((A/\\B)\\/C)"), "balanced nested", state);
    expect_false(has_balanced_brackets("((A/\\B)"), "unbalanced missing close", state);
    expect_false(has_balanced_brackets("(A/\\B))"), "unbalanced extra close", state);
}

void test_has_valid_negations(TestState& state) {
    expect_true(has_valid_negations("(!A)"), "valid neg atom", state);
    expect_true(has_valid_negations("(!(A/\\B))"), "valid neg grouped formula", state);
    expect_false(has_valid_negations("(A/\\!B)"), "invalid neg without parens", state);
    expect_false(has_valid_negations("(!A"), "invalid neg missing close", state);
}

void test_has_single_operator(TestState& state) {
    expect_true(has_single_operator("(A/\\B)"), "single operator one op", state);
    expect_true(has_single_operator("((A/\\B)\\/(C/\\D))"), "single operator top level one", state);
    expect_false(has_single_operator("(A/\\B\\/C)"), "single operator two top-level", state);
}

void test_contains(TestState& state) {
    const vector<string> values = {"A", "B", "A/\\B"};
    expect_true(contains(values, "A"), "contains existing", state);
    expect_false(contains(values, "C"), "contains missing", state);
}

void test_repeat_variables(TestState& state) {
    expect_true(repeat_variables("(A/\\B)"), "repeat variables valid", state);
    expect_false(repeat_variables("(AB\\/C)"), "repeat variables invalid AB", state);
}

void test_has_valid_operators(TestState& state) {
    expect_true(has_valid_operators("(A/\\B)"), "valid operators and", state);
    expect_true(has_valid_operators("(A\\/(B/\\C))"), "valid operators nested", state);
    expect_false(has_valid_operators("(\\/B)"), "invalid operators missing left", state);
    expect_false(has_valid_operators("(A/\\)"), "invalid operators missing right", state);
    expect_false(has_valid_operators("(A/\\#)"), "invalid operators bad right token", state);
}

void test_count_subformulas(TestState& state) {
    expect_eq_int(count_subformulas("(A/\\B)"), 3, "count simple conjunction", state);
    expect_eq_int(count_subformulas("(((!P)\\/(!Q))->((!R)\\/(!S)))"), 11, "count report example 4", state);
    expect_eq_int(count_subformulas("(((!A)/\\1)->(B\\/C))"), 8, "count report example 5", state);
}

void test_remove_spaces(TestState& state) {
    expect_eq_str(remove_spaces(" ( A /\\ B ) "), "(A/\\B)", "remove spaces", state);
}

void test_main_flow_messages(TestState& state) {
    expect_eq_str(validate_like_main("(\\/B)"), "Ошибка: Неверное использование операторов /, \\, ~ или ->!",
                  "flow invalid operators", state);
    expect_eq_str(validate_like_main("A->B)"), "Ошибка: Формула должна быть заключена в скобки!",
                  "flow missing outer brackets 1", state);
    expect_eq_str(validate_like_main("(A\\/B)->C"), "Ошибка: Формула должна быть заключена в скобки!",
                  "flow missing outer brackets 2", state);
    expect_eq_str(validate_like_main("(ABC\\/D)"),
                  "Ошибка: Недопустимы подряд идущие переменные (например, AB)!", "flow repeated vars", state);
    expect_eq_str(validate_like_main("(A/\\#)"), "Ошибка: Недопустимые символы в формуле!",
                  "flow invalid alphabet", state);
    expect_eq_str(validate_like_main("(A/\\!B)"), "Ошибка: Некорректное отрицание!", "flow invalid negation", state);
    expect_eq_str(validate_like_main(""), "Ошибка: Заполните поле для работы программы!", "flow empty", state);
    expect_eq_str(validate_like_main("(A/\\B"), "Ошибка: Формула должна быть заключена в скобки!",
                  "flow missing closing bracket", state);
    expect_eq_str(validate_like_main("((A/\\B)"), "Ошибка: Несбалансированы скобки!", "flow unbalanced", state);
    expect_eq_str(validate_like_main("5"), "Ошибка: Недопустимые символы в формуле!", "flow invalid symbol 5", state);
}

int main() {
    TestState state;

    test_is_variable(state);
    test_is_valid_alphabet(state);
    test_has_balanced_brackets(state);
    test_has_valid_negations(state);
    test_has_single_operator(state);
    test_contains(state);
    test_repeat_variables(state);
    test_has_valid_operators(state);
    test_count_subformulas(state);
    test_remove_spaces(state);
    test_main_flow_messages(state);

    cout << "Passed: " << state.passed << '\n';
    cout << "Failed: " << state.failed << '\n';
    return state.failed == 0 ? 0 : 1;
}

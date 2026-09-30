#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool is_variable(const char c) {
    return (c >= 'A' && c <= 'Z') || c == '0' || c == '1';
}

bool is_valid_alphabet(const string& str) {
    size_t i = 0;
    while (i < str.size()) {
        const char c = str[i];
        if (is_variable(c) || c == '(' || c == ')' || c == '/' || c == '\\' || c == '!' || c == '~') {
            ++i;
            continue;
        }

        if (i + 1 < str.size() && str[i] == '-' && str[i + 1] == '>') {
            i += 2;
            continue;
        }

        return false;
    }
    return true;
}

bool has_balanced_brackets(const string& str) {
    int balance = 0;
    size_t i = 0;

    while (i < str.size()) {
        if (str[i] == '(') {
            ++balance;
        } else if (str[i] == ')') {
            --balance;
            if (balance < 0) {
                return false;
            }
        }
        ++i;
    }

    return balance == 0;
}

bool has_valid_negations(const string& str) {
    size_t i = 0;
    while (i < str.size()) {
        if (str[i] != '!') {
            ++i;
            continue;
        }

        if (i > 0 && str[i - 1] == '(' && i + 2 < str.size() && str[i + 2] == ')') {
            ++i;
            continue;
        }

        if (!(i + 1 < str.size() && str[i + 1] == '(')) {
            return false;
        }

        int balance = 1;
        size_t j = i + 2;
        while (j < str.size() && balance > 0) {
            if (str[j] == '(') {
                ++balance;
            } else if (str[j] == ')') {
                --balance;
            }
            ++j;
        }

        if (balance != 0 || str[j - 1] != ')') {
            return false;
        }

        ++i;
    }
    return true;
}

bool has_single_operator(const string& str) {
    int balance = 0;
    int top_level_ops = 0;
    size_t i = 0;

    while (i < str.size()) {
        if (str[i] == '(') {
            ++balance;
        } else if (str[i] == ')') {
            --balance;
        } else if ((str[i] == '/' || str[i] == '\\') && i + 1 < str.size() && balance == 1) {
            if ((str[i] == '/' && str[i + 1] == '\\') || (str[i] == '\\' && str[i + 1] == '/')) {
                ++top_level_ops;
                ++i;
            }
        }
        ++i;
    }

    return top_level_ops <= 1;
}

bool contains(const vector<string>& vec, const string& subformula) {
    size_t i = 0;
    while (i < vec.size()) {
        if (vec[i] == subformula) {
            return true;
        }
        ++i;
    }
    return false;
}

bool repeat_variables(const string& str) {
    size_t i = 1;
    while (i < str.size()) {
        const bool f = is_variable(str[i - 1]);
        const bool s = is_variable(str[i]);
        if (f && s) {
            return false;
        }
        ++i;
    }
    return true;
}

bool has_valid_operators(const string& str) {
    size_t i = 0;
    while (i < str.size()) {
        if ((str[i] == '/' || str[i] == '\\') && i + 1 < str.size()) {
            if (!((str[i] == '/' && str[i + 1] == '\\') || (str[i] == '\\' && str[i + 1] == '/'))) {
                ++i;
                continue;
            }

            const size_t op_pos = i;
            if (op_pos == 0 || op_pos + 2 >= str.size()) {
                return false;
            }

            const char left = str[op_pos - 1];
            const char right = str[op_pos + 2];
            if (!(is_variable(left) || left == ')')) {
                return false;
            }
            if (!(is_variable(right) || right == '(' || right == '!')) {
                return false;
            }

            ++i;
        }
        ++i;
    }
    return true;
}

int count_subformulas(const string& formula) {
    vector<string> subformulas;

    size_t i = 0;
    while (i < formula.size()) {
        if (is_variable(formula[i])) {
            const string var(1, formula[i]);
            if (!contains(subformulas, var)) {
                subformulas.push_back(var);
            }
        }
        ++i;
    }

    i = 0;
    while (i < formula.size()) {
        if (formula[i] == '(') {
            int balance = 1;
            size_t j = i + 1;
            while (j < formula.size() && balance > 0) {
                if (formula[j] == '(') {
                    ++balance;
                } else if (formula[j] == ')') {
                    --balance;
                }
                ++j;
            }

            if (balance == 0) {
                const string sub = formula.substr(i + 1, j - i - 2);
                if (!sub.empty() && !contains(subformulas, sub)) {
                    subformulas.push_back(sub);
                }

                if (sub.size() == 2 && sub[0] == '!' && is_variable(sub[1])) {
                    const string neg = "!" + string(1, sub[1]);
                    if (!contains(subformulas, neg)) {
                        subformulas.push_back(neg);
                    }
                }
            }
        }
        ++i;
    }

    return static_cast<int>(subformulas.size());
}

string remove_spaces(string value) {
    value.erase(remove_if(value.begin(), value.end(), [](const unsigned char ch) { return isspace(ch); }),
                value.end());
    return value;
}

#ifndef LOIS_LAB1_TEST
int main() {
    string formula;
    while (true) {
        cout << "Введите формулу (или 9 для выхода): ";
        getline(cin, formula);
        formula = remove_spaces(formula);

        if (formula.empty()) {
            cout << "Ошибка: Заполните поле для работы программы!" << '\n';
            continue;
        }

        if (formula == "9") {
            return 0;
        }

        if (!is_valid_alphabet(formula)) {
            cout << "Ошибка: Недопустимые символы в формуле!" << '\n';
            continue;
        }

        if (!(formula.size() == 1 && is_variable(formula[0]))) {
            if (!repeat_variables(formula)) {
                cout << "Ошибка: Недопустимы подряд идущие переменные (например, AB)!" << '\n';
                continue;
            }

            if (!(formula.front() == '(' && formula.back() == ')')) {
                cout << "Ошибка: Формула должна быть заключена в скобки!" << '\n';
                continue;
            }

            if (!has_valid_operators(formula)) {
                cout << "Ошибка: Неверное использование операторов /, \\, ~ или ->!" << '\n';
                continue;
            }

            if (!has_single_operator(formula)) {
                cout << "Ошибка: Допустима только одна бинарная операция!" << '\n';
                continue;
            }

            if (!has_balanced_brackets(formula)) {
                cout << "Ошибка: Несбалансированы скобки!" << '\n';
                continue;
            }

            if (!has_valid_negations(formula)) {
                cout << "Ошибка: Некорректное отрицание!" << '\n';
                continue;
            }
        }

        const int count = count_subformulas(formula);
        cout << "Количество различных подформул: " << count << '\n';
    }
}
#endif
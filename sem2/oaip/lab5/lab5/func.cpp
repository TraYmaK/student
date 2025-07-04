#include <iostream>
#include <unordered_map>
#include <sstream>
#include "func.hpp"
#define MAX_LEN 100

using namespace std;

char substitutedRPN[MAX_LEN] = {};

int menu() {
    int operation;
    cout << "1. Продолжить" << endl;
    cout << "2. Выход" << endl;
    wall();
    cout << "Выберите операцию: ";
    cin >> operation;
    while (operation != 1 && operation != 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        wall();
        cout << "Некорректный ввод. Повторите попытку: ";
        cin >> operation;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return operation;
    wall();
}

void wall() {
	cout << "-------------------------------------------------------------------------" << endl;
}

void clear() {
	system("pause");
	system("cls");
}

void push(Node*& head, char value) {
	Node* newNode = new Node(value);
	newNode->next = head;
	head = newNode;
}

char pop(Node*& head) {
	if (head == nullptr) {
		cout << "Стек пуст.\n";
		return -1;
	}
	Node* temp = head;
	int popValue = temp->data;
	head = head->next;
	delete temp;
	return popValue;
}

int prioritization(char operation) {
	if (operation == '+' || operation == '-') return 1;
	if (operation == '*' || operation == '/') return 2;
	return 0;
}

bool validateExpression(const char* expression) {
    int len = strlen(expression);
    bool lastWasOp = true;
    bool usedVars[256] = { false };
    int balance = 0;

    for (int i = 0; i < len; ++i) {
        char c = expression[i];

        if (c == ' ' || c == '\t') continue;

        if (c == '(') {
            balance++;
            lastWasOp = true;
        }
        else if (c == ')') {
            balance--;
            if (balance < 0) return false;
            lastWasOp = false;
        }
        else if (isalpha(c)) {
            if (!lastWasOp) return false;
            lastWasOp = false;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (lastWasOp) return false;
            lastWasOp = true;
        }
        else {
            return false;
        }
    }

    return !lastWasOp && balance == 0;
}

void convertation(const char* expression, char* RVNexpression) {
    Node* head = nullptr;
    int outIndex = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (c == ' ' || c == '\t')
            continue;

        if (isalpha(c)) {
            RVNexpression[outIndex++] = c;
        }
        else if (c == '(') {
            push(head, c);
        }
        else if (c == ')') {
            while (head != nullptr && head->data != '(') {
                RVNexpression[outIndex++] = pop(head);
            }
            if (head != nullptr) pop(head);
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (head != nullptr && (prioritization(head->data) >= prioritization(c))) {
                RVNexpression[outIndex++] = pop(head);
            }
            push(head, c);
        }
    }
    while (head != nullptr) {
        RVNexpression[outIndex++] = pop(head);
    }
    substitutedRPN[MAX_LEN - 1] = '\0';
}

void takeExpression(char* expression) {
    cout << "Введите выражение: ";
    cin.getline(expression, MAX_LEN);
}

void printExpression(char* expression) {
    cout << "Изначальное выражение: " << expression << endl;
}

void printRPNExpression(char* RVNexpression) {
    cout << "Обратная польская запись: " << RVNexpression << endl;
}

void calcRPN(const char* RPNexpression) {
    double stack[MAX_LEN] = { 0 };
    int head = -1;

    stringstream ss(RPNexpression);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1) || token.find('.') != string::npos) {
            stack[++head] = stod(token);
        }
        else {
            if (head < 1) {
                cout << "Ошибка: Недостаточно операндов в стеке.\n";
                return;
            }

            double b = stack[head--];
            double a = stack[head--];
            double result = 0;

            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else if (token == "/") {
                if (b == 0) {
                    cout << "Ошибка: Деление на ноль " << endl;
                    return;
                }
                result = a / b;
            }
            else {
                cout << "Ошибка: Неизвестная операция " << token << endl;
                return;
            }

            stack[++head] = result;
        }
    }

    if (head != 0) {
        cout << "Ошибка: Некорректное выражение в ОПЗ.\n";
        return;
    }

    cout << "Результат вычисления ОПЗ: " << stack[head] << endl;
}

void substituteValues(char* RPNexpression, char* substitutedRPN) {
    unordered_map<char, double> values;
    char tempExpression[MAX_LEN] = {};
    int index = 0;

    for (int i = 0; RPNexpression[i] != '\0'; i++) {
        char c = RPNexpression[i];

        if (isalpha(c) && values.find(c) == values.end()) {
            cout << "Введите значение переменной " << c << ": ";
            while (!(cin >> values[c])) {
                cout << "Ошибка: Введите число для " << c << ": ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        if (isalpha(c)) {
            double val = values[c];
            char num[20];   
            sprintf_s(num, "%.1f", val);

            for (int j = 0; num[j] != '\0'; j++) {
                tempExpression[index++] = num[j];
            }
            tempExpression[index++] = ' ';
        }
        else {
            tempExpression[index++] = c;
            tempExpression[index++] = ' ';
        }
    }
    tempExpression[index] = '\0';
    strcpy_s(substitutedRPN, MAX_LEN, tempExpression);
}

void method(char* expression, char* RPNexpression, char* substitutedRPN) {
    while (true) {
        wall();
        takeExpression(expression);
        wall();
        printExpression(expression);
        wall();
        if (!validateExpression(expression)) {
            cout << "Ошибка: Некорректное выражение." << endl;
            continue;
        }
        convertation(expression, RPNexpression);
        printRPNExpression(RPNexpression);
        wall();
        substituteValues(RPNexpression, substitutedRPN);
        wall();
        calcRPN(substitutedRPN);
        wall();
        int operation = menu();
        if (operation == 1) {
            wall();
            clear();
            continue;
        }
        else if (operation == 2) {
            wall();
            exit(0);
        }
        clear();
    }
}
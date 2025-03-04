#include <iostream>
#include "func.hpp"
#define MAX_LEN 100

using namespace std;

int menu() {
    int operation;
    cout << "1. ����������" << endl;
    cout << "2. �����" << endl;
    wall();
    cout << "�������� ��������: ";
    cin >> operation;
    while (operation != 1 && operation != 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        wall();
        cout << "������������ ����. ��������� �������: ";
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
		cout << "���� ����.\n";
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

void convertation(const char* expression, char* RVNexpression) {
    Node* head = nullptr;
    int outIndex = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (c == ' ' || c == '\t')
            continue;

        if (isdigit(c)) {
            RVNexpression[outIndex++] = c;
        }
        else if (c == '(') {
            push(head, c);
        }
        else if (c == ')') {
            while (head != nullptr && head->data != '(') {
                RVNexpression[outIndex++] = pop(head);
            }
            if (head != nullptr && head->data == '(') {
                pop(head);
            }
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
    RVNexpression[outIndex] = '\0';
}

void takeExpression(char* expression) {
    cout << "������� ���������: ";
    cin.getline(expression, MAX_LEN);
}

void printExpression(char* expression) {
    cout << "����������� ���������: " << expression << endl;
}

void printRPNExpression(char* RVNexpression) {
    cout << "�������� �������� ������: " << RVNexpression << endl;
}

void method(char* expression, char* RPNexpression) {
    while (true) {
        wall();
        takeExpression(expression);
        wall();
        printExpression(expression);
        wall();
        convertation(expression, RPNexpression);
        printRPNExpression(RPNexpression);
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
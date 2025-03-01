#include <iostream>
#include "func.hpp"

using namespace std;

struct Node {
	int data;
	Node* next;

	Node(int value) {
		data = value;
		next = nullptr;
	}
};

void wall() {
	cout << "-------------------------------------------------------------------------" << endl;
}

void clear() {
	system("pause");
	system("cls");
}

void push(Node*& head, int value) {
	Node* newNode = new Node(value);
	newNode->next = head;
	head = newNode;
}

int pop(Node*& head) {
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

void displayStack(Node* head) {
	wall();
	cout << "Стек: ";
	Node* current = head;
	while (current) {
		cout << current->data << " ";
		current = current->next;
	}
	cout << endl;
}

void clearStack(Node*& head) {
	while (head) {
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}

void averageCalculating(Node*& head, double& average) {
	Node* current = head;
	int sum = 0;
	int counter = 0;
	while (current) {
		sum += current->data;
		counter++;
		current = current->next;
	}
	if (counter > 0) {
		average = sum / counter;
	}
	else {
		average = 0;
	}
}

void averageReplace(Node*& head, double& average) {
	Node* current = head;
	current = head;
	while (current) {
		if (current->data % 2 == 0) {
			current->data = static_cast<int>(average);
		}
		current = current->next;
	}
}

void menu(int stackSize, Node*& head) {
	wall();
	cout << "Введите изначальное кол-во элементов стека: ";
	cin >> stackSize;
	wall();

	for (int i = 0; i < stackSize; i++) {
		int value, counter;
		cout << "Введите элемент " << i + 1 << ": ";
		cin >> value;
		push(head, value);
		wall();
	}
}

void method(int& choice, Node*& head, double& average) {
	wall();
	cout << "Операции: \n";
	wall();
	cout << "1. Вычислить среднее арифметическое и заменить им все четные элементы \n";
	cout << "2. Выход \n";
	wall();
	cout << "Выберите операцию: ";
	cin >> choice;
	wall();
	if (choice == 1) {
		clear();
		displayStack(head);
		averageCalculating(head, average);
		averageReplace(head, average);
		displayStack(head);
		wall();
	}
	else if (choice == 2) {
		exit(0);
	}
	else {
		clear();
		displayStack(head);
		method(choice, head, average);
	}
}
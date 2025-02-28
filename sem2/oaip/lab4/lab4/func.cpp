#include <iostream>
#include "func.hpp"

using namespace std;

struct Node {
	int data;
	Node* next;
	Node* prev;

	Node(int value) {
		data = value;
		next = nullptr;
		prev = nullptr;
	}
};

void wall() {
	cout << "-------------------------------------------------------------------------" << endl;
}

void clear() {
	system("pause");
	system("cls");
}

void pushFace(Node*& head, Node*& end, int value) {
	Node* newNode = new Node(value);
	if (!head) {
		head = end = newNode;
	}
	else {
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
}

void pushBack(Node*& head, Node*& end, int value) {
	Node* newNode = new Node(value);
	if (!end) {
		head = end = newNode;
	}
	else {
		end->next = newNode;
		newNode->prev = end;
		end = newNode;
	}
}

void userPushFace(Node*& head, Node*& end, int value) {
	wall();
	cout << "Введите элемент: ";
	cin >> value;
	pushFace(head, end, value);
}

void userPushBack(Node*& head, Node*& end, int value) {
	wall();
	cout << "Введите элемент: ";
	cin >> value;
	pushBack(head, end, value);
}

int popFace(Node*& head, Node*& end, int value) {
	if (head == nullptr) {
		cout << "Стек пуст.\n";
		return -1;
	}
	int poppedFaceValue = head->data;
	Node* temp = head;
	head = head->next;
	if (head) {
		head->prev = nullptr;
	}
	else {
		end = nullptr;
	}
	delete temp;
	wall();
	cout << "Извлеченный элемент: " << poppedFaceValue << endl;
	return poppedFaceValue;
}

int popBack(Node*& head, Node*& end, int value) {
	if (end == nullptr) {
		cout << "Стек пуст.\n";
		return -1;
	}
	int poppedBackValue = end->data;
	Node* temp = end;
	end = end->prev;
	if (end) {
		end->next = nullptr;
	}
	else {
		head = nullptr;
	}
	delete temp;
	wall();
	cout << "Извлеченный элемент: " << poppedBackValue << endl;
	return poppedBackValue;
}

void displayStackFace(Node*& head, Node*& end, int value) {
	Node* temp = head;
	wall();
	cout << "Стек ->: ";
	while (temp) {
		cout << temp->data << " ";
		temp = temp->next;
	}
	cout << endl;
}

void displayStackBack(Node*& head, Node*& end, int value) {
	Node* temp = end;
	wall();
	cout << "Стек <-: ";
	while (temp) {
		cout << temp->data << " ";
		temp = temp->prev;
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

void averageFaceReplace(Node*& head, double& average) {
	Node* current = head;
	current = head;
	while (current) {
		if (current->data % 2 == 0) {
			current->data = average;
		}
		current = current->next;
	}
}

void averageBackReplace(Node*& end, double& average) {
	Node* current = end;
	while (current) {
		if (current->data % 2 == 0) {
			current->data = average;
		}
		current = current->prev;
	}
}

void menu(Node*& head, Node*& end, int stackSize, int value) {
	wall();
	cout << "Введите изначальное кол-во элементов стека: ";
	cin >> stackSize;
	wall();

	for (int i = 0; i < stackSize; i++) {
		int value;
		cout << "Введите элемент " << i + 1 << ": ";
		cin >> value;
		pushFace(head, end, value);
		wall();
	}
}

void method(Node*& head, Node*& end, double& average, int& choice, int& value, int& poppedFaceValue, int& poppedBackValue) {
	while (true) {
		displayStackFace(head, end, value);
		wall();
		cout << "Операции: \n";
		wall();
		cout << "1. Добавить элемент спереди \n";
		cout << "2. Добавить элемент сзади \n";
		cout << "3. Извлечь и вывести элемент спереди \n";
		cout << "4. Извлечь и вывести элемент сзади \n";
		cout << "5. Вывести стек спереди \n";
		cout << "6. Вывести стек сзади \n";
		cout << "7. Вычислить среднее арифметическое и заменить им все четные элементы спереди \n";
		cout << "8. Вычислить среднее арифметическое и заменить им все четные элементы сзади \n";
		cout << "9. Выход \n";
		wall();
		cout << "Выберите операцию: ";
		cin >> choice;
		wall();
		if (choice == 1) {
			clear();
			displayStackFace(head, end, value);
			userPushFace(head, end, value);
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 2) {
			clear();
			displayStackFace(head, end, value);
			userPushBack(head, end, value);
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 3) {
			clear();
			displayStackFace(head, end, value);
			popFace(head, end, value);
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 4) {
			clear();
			displayStackFace(head, end, value);
			popBack(head, end, value);
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 5) {
			clear();
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 6) {
			clear();
			displayStackBack(head, end, value);
			wall();
		}
		else if (choice == 7) {
			clear();
			displayStackFace(head, end, value);
			averageCalculating(head, average);
			averageFaceReplace(head, average);
			displayStackFace(head, end, value);
			wall();
		}
		else if (choice == 8) {
			clear();
			displayStackBack(head, end, value);
			averageCalculating(head, average);
			averageBackReplace(end, average);
			displayStackBack(head, end, value);
			wall();
		}
		else if (choice == 9) {
			exit(0);
		}
		else {
			clear();
			displayStackFace(head, end, value);
			method(head, end, average, choice, value, poppedFaceValue, poppedBackValue);
		}
		clear();
	}
}
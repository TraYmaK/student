#include <iostream>
#include "func.hpp"

using namespace std;

int main() {
	setlocale(LC_ALL, "ru");

	Node* head = nullptr;
	int stackSize = 0;
	int choice = 0;
	double average = 0;

	menu(stackSize, head);
	clear();
	displayStack(head);
	method(choice, head, average);
	clearStack(head);

	return 0;
}
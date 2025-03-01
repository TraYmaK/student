#include <iostream>
#include "func.hpp"

using namespace std;

int main() {
	setlocale(LC_ALL, "ru");

	Node* head = nullptr;
	Node* end = nullptr;
	int stackSize = 0;
	int choice = 0;
	int value = 0;
	int poppedFaceValue = 0;
	int poppedBackValue = 0;
	double average = 0;

	menu(head, end, stackSize, value);
	clear();
	method(head, end, average, choice, value, poppedFaceValue, poppedBackValue);
	clearStack(head);

	return 0;
}
#include <iostream>

using namespace std;

struct Object {
	bool number;
	double value;
	char operation;
};

int prioritization(char operation) {
	if (operation == '+' || operation == '-') return 1;
	if (operation == '*' || operation == '/') return 2;
	return 0;
}

int main() {

}
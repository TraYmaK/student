#include <iostream>
#include <math.h>

using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

	double x, y, z, h;

	cout << "Введите x: ";
	cin >> x;
	cout << "Введите y: ";
	cin >> y;
	cout << "Введите z: ";
	cin >> z;

	double ExpressionValue = (1 + x * abs(y - tan(z)));

	if (x == y && y == z && x == z) {
		cout << "Ошибка: ввод одинаковых значений";
		exit(1);
	}
	else {
		if (ExpressionValue == 0) {
			cout << "Ошибка: деление на 0";
			exit(2);
		}
		else {
			h = ((pow(x, y + 1) + pow(exp(1), y - 1)) / (1 + x * abs(y - tan(z))) * (1 + abs(y - x)) + (pow(abs(y - x), 2) / 2) - (pow(abs(y - x), 3) / 3));
			h = round(h * 100000) / 100000;
			cout << h;
			return 0;
		}
	}
}
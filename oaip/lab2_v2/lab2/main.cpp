#include <iostream>
#include <math.h>

using namespace std;
class predel {
public:
	double max(double a, double b) {
		return (a > b) ? a : b;
	}

	double min(double a, double b) {
		return (a < b) ? a : b;
	}
};

double DivisionByZeroCheck(double x) {
	double MeaningOfTheExpression = 5 - 2 * x;
	if (MeaningOfTheExpression == 0) {
		cout << "Ошибка: деление на 0";
		exit(1);
	}
	return 0;
}

int main() {
	setlocale(LC_ALL, "RU");

	double x, y, f;
	const double pi = 3.14159265358979323846;


	cout << "Введите число x: ";
	cin >> x;
	cout << "Введите число y: ";
	cin >> y;
	
	predel predel;

	if (x <= 0) {
		f = predel.min(0.9 * y, pow(exp(1), (2 * x - 3)));
		cout << f;
	}
	else if (x >= 0 && y > 0 && !DivisionByZeroCheck(x)) {
		f = ((2 * cos(x - pi / 6)) + cbrt(y) / (5 - 2 * x));
		cout << f;
	}
	else {
		f = predel.max(pow(sin(y), 2), pow(cos(y), 2));
		cout << f;
	}
	return 0;
}
#include <iostream>
#define _USE_MATH_DEFINES
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

class errors {
public:
	double DivisionByZeroCheck(double x) {
		double MeaningOfTheExpression = 5 - 2 * x;
		if (MeaningOfTheExpression == 0) {
			cout << "Ошибка: деление на 0";
			exit(1);
		}
		return 0;
	}

	double EnteringIdenticalValues(double x,double y) {
		if (x == y) {
			cout << "Ошибка: ввод одинаковых данных";
			exit(2);
		}
		return 0;
	}
};

int main() {
	setlocale(LC_ALL, "RU");

	double x, y, f;

	cout << "Введите число x: ";
	cin >> x;
	cout << "Введите число y: ";
	cin >> y;
	
	predel predel;
	errors errors;

	errors.EnteringIdenticalValues(x,y);

	if (x <= 0) {
		f = predel.min(0.9 * y, pow(exp(1), (2 * x - 3)));
		cout << f;
	}
	else if (x >= 0 && y > 0 && !errors.DivisionByZeroCheck(x)) {
		f = ((2 * cos(x - M_PI / 6)) + cbrt(y) / (5 - 2 * x));
		cout << f;
	}
	else {
		f = predel.max(pow(sin(y), 2), pow(cos(y), 2));
		cout << f;
	}
	return 0;
}
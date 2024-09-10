#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

double max(double a, double b) {
    return (a > b) ? a : b;
}

double min(double a, double b) {
    return (a < b) ? a : b;
}

int main() {
    setlocale(LC_ALL, "RU");

    double x, y, f;

    cout << "Введите число x: ";
    cin >> x;
    cout << "Введите число y: ";
    cin >> y;

    if (x <= 0) {
        f = min(0.9 * y, pow(exp(1), (2 * x - 3)));
        cout << f;
    }
    else if (x >= 0 && y > 0) {
        if (5 - 2 * x == 0) {
            cout << "Ошибка: деление на 0" << endl;
            return 1;
        }
        f = ((2 * cos(x - M_PI / 6)) + cbrt(y) / (5 - 2 * x));
        cout << f;
    }
    else {
        f = max(pow(sin(y), 2), pow(cos(y), 2));
        cout << f;
    }

    return 0;
}
#include <iostream>
#include <cmath>

using namespace std;

double Y(double x) {
    return -log(sqrt(1 + x * x)) + x * atan(x);
}

void wall() {
    cout << "--------------------------------------------------------------------\n";
}

int main() {
    setlocale(LC_ALL, "RU");

    double a, b, h, eps, s, y, x, mod;
    int counter;

    cout << "Введите начало: ";
    cin >> a;
    cout << "Введите конец: ";
    cin >> b;
    cout << "Введите шаг: ";
    cin >> h;
    cout << "Введите погрешность: ";
    cin >> eps;
    cout << endl;

    cout << fixed;
    cout.precision(5);
    wall();
    cout << "|   x   |    S(x)   |   Y(x)   |   |Y(x)-S(x)|   |   Кол-во сумм   |\n";
    wall();

    for (double x = a; x <= b; x += h) {

        int k = 1;
        double r = x * x;
        s = x * x / 2;
        y = Y(x);

        while (fabs(s - y) >= eps) {
            k++;
            r *= (-1) * x * x;
            s += r / (2 * k) / (2 * k - 1);

        }

        mod = abs(y - s);

        cout << "|" << x << "|  "
            << s << "  |  "
            << y << " |     "
            << mod << "     |        ";
        if (k - 1 < 10) {
            cout << k - 1 << "        |" << endl;
        }
        else {
            cout << k - 1 << "       |" << endl;
        }

    }

    wall();

    return 0;
}
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

    for (double i = a; i <= b; i += h) {

        int k = 1;
        counter = 0;
        s = 0.0;
        x = i;

        y = Y(x);

        while (abs(s - y) >= eps) {
            s += pow(-1, k + 1) * pow(x, 2 * k) / (2 * k * (2 * k - 1));
            k++;
            counter++;
        }

        mod = abs(y - s);

        cout << "|" << x << "|  "
            << s << "  |  "
            << y << " |     "
            << mod << "     |        ";
        if (counter < 10) {
            cout << counter << "        |" << endl;
        }
        else {
            cout << counter << "       |" << endl;
        }

    }

    wall();

    return 0;
}

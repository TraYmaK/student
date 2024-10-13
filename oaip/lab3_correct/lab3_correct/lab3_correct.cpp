#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double Y(double x) {
    return -log(sqrt(1 + pow(x, 2))) + x * atan(x);
}

int main() {
    setlocale(LC_ALL, "RU");

    double a, b, h, epsilon, s1, y1, x;
    int s4et, t;

    cout << "Введите a: ";
    cin >> a;
    cout << "Введите b: ";
    cin >> b;
    cout << "Введите шаг h: ";
    cin >> h;
    cout << "Введите погрешность epsilon: ";
    cin >> epsilon;
    cout << endl;

    cout << fixed;
    cout.precision(5);
    cout << "        x|        S(x)|        Y(x)|   |Y(x)-S(x)|| Кол-во сумм\n";
    cout << "-------------------------------------------------------------\n";

    for (double i = a; i <= b; i += h) {
        s4et = 1;
        t = 0;
        x = i;
        y1 = Y(x);
        s1 = 0.0;

        double comp = x * x;

        while (abs(s1 - y1) >= epsilon) {
            s1 += pow(-1, s4et + 1) * comp / (2 * s4et * (2 * s4et - 1));
            comp *= x * x;
            s4et++;
            t++;
        }

        cout << setw(8) << x << " | "
             << setw(10) << s1 << " | "
             << setw(10) << y1 << " | "
             << setw(12) << abs(y1 - s1) << " | "
             << setw(7) << t << endl;
    }

    return 0;
}

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

void wall() {
    cout << "-----------------------------------------------------------------------\n";
}

double sqrtIterative(double a, int n) {
    double x = 0.5 * (1.0 + a);
    for (int i = 0; i < n; ++i) {
        x = 0.5 * (x + a / x);
    }
    return x;
}

double sqrtRecursive(double a, int n, double x = -1) {
    if (x == -1) {
        x = 0.5 * (1.0 + a);
    }
    if (n == 0) {
        return x;
    }
    return sqrtRecursive(a, n - 1, 0.5 * (x + a / x));
}


int main() {
    setlocale(LC_ALL, "RU");

    double a;
    int n;

    wall();

    cout << "Введите число a: ";
    cin >> a;

    wall();

    cout << "Введите количество итераций n: ";
    cin >> n;

    cout << fixed << setprecision(10);

    wall();
    cout << "|     n      |   Iterative sqrt   |   Recursive sqrt   |    |IY-RY|   |\n";
    wall();

    double xIter = 0.5 * (1.0 + a);
    double xRec = 0.5 * (1.0 + a);

    for (int i = 0; i < n; ++i) {
        xIter = 0.5 * (xIter + a / xIter);
        xRec = sqrtRecursive(a, 1, xRec);

        double diff = fabs(xIter - xRec);
        cout << "| " << setw(10) << (i + 1)
            << " | " << setw(18) << xIter
            << " | " << setw(18) << xRec
            << " | " << setw(10) << diff << " |\n";
    }

    wall();

    return 0;
}
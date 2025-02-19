#include <iostream>
#include <cmath>
using namespace std;

double sqrtIterative(double a, int iterations) {
    double x = 0.5 * (1.0 + a);
    for (int i = 0; i < iterations; ++i) {
        x = 0.5 * (x + a / x);
    }
    return x;
}

double sqrtRecursiveHelper(double a, double x, int currentStep, int maxSteps) {
    if (currentStep >= maxSteps) {
        return x;
    }
    double xNext = 0.5 * (x + a / x);
    return sqrtRecursiveHelper(a, xNext, currentStep + 1, maxSteps);
}

double sqrtRecursive(double a, int iterations) {
    double x0 = 0.5 * (1.0 + a);
    return sqrtRecursiveHelper(a, x0, 0, iterations);
}

void wall() {
    cout << "------------------------------------------------------------\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    wall();
    cout << "Введите число: ";
    double a;
    cin >> a;
    wall();
    cout << "Введите количество итераций: ";
    int n;
    cin >> n;
    wall()
    double xIter = sqrtIterative(a, n);
    double xRec = sqrtRecursive(a, n);

    cout << "\nИтеративный метод:  " << xIter << endl;
    cout << "Рекурсивный метод:  " << xRec << endl;

    cout << "\nПогрешность (итеративный - sqrt): " << fabs(xIter - xLib) << endl;
    cout << "Погрешность (рекурсивный - sqrt):  " << fabs(xRec - xLib) << endl;

    return 0;
}

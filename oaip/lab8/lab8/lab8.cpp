#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double SI(int n, double eps) {
    double prev = 0.0;
    double result = 1.0 / (1.0 + prev);

    for (int i = 2; i <= n; ++i) {
        prev = result;
        result = 1.0 / (1.0 + prev);

        if (fabs(result - prev) < eps) {
            break;
        }
    }
    return result;
}

double YR(int n) {
    if (n == 1) {
        return 1.0;
    }
    return 1.0 / (1.0 + YR(n - 1));
}

void wall() {
    cout << "------------------------------------------------------------\n";
}

int main() {
    setlocale(LC_ALL, "RU");

    int n;
    double eps;

    cout << "Введите максимальное число ступеней n: ";
    cin >> n;
    cout << "Введите точность (eps): ";
    cin >> eps;

    cout << fixed << setprecision(10);
    wall();
    cout << "|    n    | Iterative Y(n) | Recursive Y(n) |    |IY-RY|   |\n";
    wall();

    for (int i = 1; i <= n; ++i) {
        double IterativeS = SI(i, eps);
        double RecursiveY = YR(i);
        double diff = fabs(IterativeS - RecursiveY);

        cout << "| " << setw(7) << i
            << " | " << setw(14) << IterativeS
            << " | " << setw(14) << RecursiveY
            << " | " << setw(10) << diff << " |\n";
    }

    wall();

    return 0;
}

#include <iostream>
#include <math.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    
    double a, b, h;
    int n;

    cout << "Введите значения a, b, h и n (через пробел): ";
    cin >> a >> b >> h >> n;

    
    cout << "x\tY(x)\tS(x)\t|Y(x) - S(x)|" << endl;
    cout.precision(6);

    
    for (double x = a; x <= b; x += h) {
        
        double Y = -log(sqrt(1 + x * x)) + x * atan(x);

        
        double S = 0;
        for (int k = 1; k <= n; ++k) {
            double term = pow(-1, k + 1) * pow(x, 2 * k) / (2 * k * (2 * k - 1));
            S += term;
        }

        
        cout << x << "\t" << Y << "\t" << S << "\t" << fabs(Y - S) << endl;
    }

    return 0;
}
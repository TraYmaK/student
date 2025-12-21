#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    int n, m, experts;
    cout << "Введите количество альтернатив: ";
    cin >> n;
    cout << "Введите количество критериев: ";
    cin >> m;
    cout << "Введите количество экспертов: ";
    cin >> experts;

    if (n <= 0 || m <= 0 || experts <= 0) {
        cout << "Некорректные данные.\n";
        return 0;
    }

    vector<double> w(m);
    cout << "\nВведите веса критериев (сумма примерно 1):\n";
    for (int j = 0; j < m; j++) {
        cout << "Вес критерия " << j + 1 << ": ";
        cin >> w[j];
    }

    vector<vector<double>> avg(n, vector<double>(m, 0.0));

    for (int e = 0; e < experts; e++) {
        cout << "\nЭксперт " << e + 1 << ":\n";
        for (int i = 0; i < n; i++) {
            cout << "Альтернатива " << i + 1 << ":\n";
            for (int j = 0; j < m; j++) {
                double s;
                cout << "  Балл по критерию " << j + 1 << " (1-5): ";
                cin >> s;
                if (s < 1.0) s = 1.0;
                if (s > 5.0) s = 5.0;
                avg[i][j] += s;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            avg[i][j] /= experts;
        }
    }

    vector<double> add(n, 0.0);
    vector<double> mult(n, 1.0);

    for (int i = 0; i < n; i++) {
        double a = 0.0;
        double p = 1.0;
        for (int j = 0; j < m; j++) {
            double q = avg[i][j] / 5.0;
            a += w[j] * q;
            if (q < 1e-6) q = 1e-6;
            p *= pow(q, w[j]);
        }
        add[i] = a;
        mult[i] = p;
    }

    cout << fixed << setprecision(6);

    cout << "\nСредние баллы:\n";
    for (int i = 0; i < n; i++) {
        cout << "Альтернатива " << i + 1 << ": ";
        for (int j = 0; j < m; j++) {
            cout << avg[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\nЗначения функций:\n";
    for (int i = 0; i < n; i++) {
        cout << "Альтернатива " << i + 1
             << " | аддитивная = " << add[i]
             << " | мультипликативная = " << mult[i] << "\n";
    }

    int bestAdd = 0, bestMult = 0;
    for (int i = 1; i < n; i++) {
        if (add[i] > add[bestAdd]) bestAdd = i;
        if (mult[i] > mult[bestMult]) bestMult = i;
    }

    cout << "\nЛучший по аддитивной модели: альтернатива "
         << bestAdd + 1 << " (значение = " << add[bestAdd] << ")\n";
    cout << "Лучший по мультипликативной модели: альтернатива "
         << bestMult + 1 << " (значение = " << mult[bestMult] << ")\n";

    return 0;
}
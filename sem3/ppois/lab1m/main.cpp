#include <iostream>
#include <string>
#include "multiset.hpp"

using namespace std;

int main() {
    string input;
    cout << "Enter multiset: ";
    getline(cin, input);

    Multiset m(input);
    cout << "Parsed: " << m.toString() << endl;

    return 0;
}

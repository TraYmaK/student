#include <iostream>
#include <string>
#include <limits>

using namespace std;

void wall() {
    cout << "-----------------------------------------------------";
}

void clear() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("clear");
}

void enter_numbers(int& number1, int& number2, int& first_positive_index, int& second_positive_index) {
    while (true) {
        wall();
        cout << "\nEnter 1st number: ";
        cin >> number1;
        if (number1 < 0 || number1 > 127) {
            wall();
            cout << "\nYour number outside the interval from 0 to 127\n";
            wall();
            cout << "\nPress enter to continue...\n";
            wall();
            clear();
            continue;
        }
        wall();
        cout << "\nYour number is positive? (yes = 1 or no = 0): ";
        cin >> first_positive_index;
        if (first_positive_index != 0 && first_positive_index != 1) {
            wall();
            cout << "\nPositive index should be yes = 1 or no = 0\n";
            wall();
            cout << "\nPress enter to continue...\n";
            wall();
            clear();
            continue;
        }
        wall();
        cout << "\nEnter 2nd number: ";
        cin >> number2;
        if (number2 < 0 || number2 > 127) {
            cout << "\nYour number outside the interval from 0 to 127\n";
            wall();
            cout << "\nPress enter to continue...\n";
            wall();
            clear();
            continue;
        }
        wall();
        cout << "\nYour number is positive? (yes = 1 or no = 0): ";
        cin >> second_positive_index;
        if (second_positive_index != 0 && second_positive_index != 1) {
            wall();
            cout << "\nPositive index should be yes = 1 or no = 0\n";
            wall();
            cout << "\nPress enter to continue...\n";
            wall();
            clear();
            continue;
        }
        wall();
        cout << "\nPress enter to continue...\n";
        wall();
        clear();
        wall();
        break;
    }
}

void display_number(int number, int positive_index, const string& order) {
    if (positive_index == 1) {
        cout << endl << order << " number: +" << number;
    }else if (positive_index == 0) {
        cout << endl << order << " number: -" << number;
    }
    cout << endl;
    wall();
}

int calculate_bit_depth(int number) {
    if (number <= 1) return  1;
    if (number <= 3) return 2;
    if (number <= 7) return 3;
    if (number <= 15) return 4;
    if (number <= 31) return 5;
    if (number <= 63) return 6;
    return 7;
}

void display_bit_depth(int bit_depth, const string& order) {
    cout << endl << order << " bit depth:                       " << bit_depth << endl;
}

void create_and_fill_binary(int bit_depth, int number, int number_bin[]) {
    int number_loop = number;
    for (int i = bit_depth - 1; i >= 0; i--) {
        number_bin[i] = number_loop % 2;
        number_loop = number_loop / 2;
    }
}

void display_binary_form(int bit_depth, int number_bin[], const string& order) {
    cout << "Binary form of " << order << " number:           ";
    for (int i = 0 ; i < bit_depth; i++) {
        cout << number_bin[i] << " ";
    }
}

void create_and_fill_eight_bit(int eight_bit, int bit_depth, const int number_bin[], int number_eight_bit[]) {
    for (int i = 0 ; i < eight_bit; i++) {
        number_eight_bit[i] = 0;
    }
    for (int i = eight_bit - bit_depth, j = 0; i < eight_bit; i++, j++) {
        number_eight_bit[i] = number_bin[j];
    }
}

void display_eight_bit(int eight_bit, int number_eight_bit[], const string& order) {
    cout << "\n8-bit module form of " << order << " number:     ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit[i] << " ";
    }
}

void create_and_fill_direct(int eight_bit, int number_eight_bit_direct[], const int number_eight_bit[]) {
    for (int i = 0; i < eight_bit; i++) {
        number_eight_bit_direct[i] = number_eight_bit[i];
    }
    number_eight_bit_direct[0] = 1;
}

void display_direct(int eight_bit, int number_eight_bit_direct[], const string& order) {
    cout << "\n8-bit direct form of " << order << " number:     ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_direct[i] << " ";
    }
}

void create_and_fill_back(int number_eight_bit_back[], int eight_bit, const int number_eight_bit_direct[]) {
    number_eight_bit_back[0] = 1;
    for (int i = 1; i < eight_bit; i++) {
        if (number_eight_bit_direct[i] == 1) {
            number_eight_bit_back[i] = 0;
        }else if (number_eight_bit_direct[i] == 0) {
            number_eight_bit_back[i] = 1;
        }
    }
}

void display_back(int eight_bit, int number_eight_bit_back[], const string& order) {
    cout << "\n8-bit back form of " << order << " number:       ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_back[i] << " ";
    }
}

void create_and_fill_additional(int eight_bit, int number_eight_bit_additional[], const int number_eight_bit_back[]) {
    for (int i = 0; i < eight_bit; i++) {
        number_eight_bit_additional[i] = number_eight_bit_back[i];
    }
    int count = 0;
    while (count < eight_bit) {
        count++;
        int i = eight_bit - count;
        if (number_eight_bit_back[i] == 1) {
            number_eight_bit_additional[i] = 0;
        }else if (number_eight_bit_back[i] == 0) {
            number_eight_bit_additional[i] = 1;
            break;
        }
    }
}

void display_additional(int eight_bit, int number_eight_bit_additional[], const string& order) {
    cout << "\n8-bit additional form of " << order << " number: ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_additional[i] << " ";
    }
    cout << endl;
    wall();
}

void create_and_fill_positive_direct(int eight_bit, int const number_eight_bit[], int number_eight_bit_direct[]) {
    for (int i = 0; i < eight_bit; i++) {
        number_eight_bit_direct[i] = number_eight_bit[i];
    }
}

void display_positive_direct(int eight_bit, int number_eight_bit_direct[], const string& order) {
    cout << "\n8-bit direct form of " << order << " number:     ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_direct[i] << " ";
    }
    cout << endl;
    wall();
}

void create_and_fill_positive_back(int eight_bit, int const number_eight_bit[], int number_eight_bit_back[]) {
    for (int i = 0; i < eight_bit; i++) {
        number_eight_bit_back[i] = number_eight_bit[i];
    }
}

void display_positive_back(int eight_bit, int number_eight_bit_back[], const string& order) {
    cout << "\n8-bit back form of " << order << " number:       ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_back[i] << " ";
    }
}

void create_and_fill_positive_additional(int eight_bit, int const number_eight_bit[], int number_eight_bit_additional[]) {
    for (int i = 0; i < eight_bit; i++) {
        number_eight_bit_additional[i] = number_eight_bit[i];
    }
}

void display_positive_additional(int eight_bit, int number_eight_bit_additional[], const string& order) {
    cout << "\n8-bit additional form of " << order << " number:     ";
    for (int i = 0; i < eight_bit; i++) {
        cout << number_eight_bit_additional[i] << " ";
    }
    cout << endl;
    wall();
}

void summ_direct(int number1, int number2, int first_positive_index, int second_positive_index, int eight_bit, int summ_direct_result[], int const first_number_eight_bit_direct[], int first_number_eight_bit_direct_sum[], int const second_number_eight_bit_direct[], int second_number_eight_bit_direct_sum[]) {
    for (int i = eight_bit - 1; i >= 0; i--) {
        first_number_eight_bit_direct_sum[i] = first_number_eight_bit_direct[i];
        second_number_eight_bit_direct_sum[i] = second_number_eight_bit_direct[i];
    }

    if (first_positive_index == 1 && second_positive_index == 1) {

        int count = 0;

        for (int i = eight_bit - 1; i >= 0; i--) {
            if (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 0 && count != 0) {
                summ_direct_result[i] = 1;
                count--;
            } else if ((first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 0 && count != 0) || (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 1 && count != 0)) {
                summ_direct_result[i] = 0;
                count--;
            } else if ((first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 1 && count != 0) || (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 0) || (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 1)) {
                summ_direct_result[i] = 1;
            } else if (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 0) {
                summ_direct_result[i] = 0;
            } else if (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 1) {
                summ_direct_result[i] = 0;
                count++;
            }
        }
        summ_direct_result[0] = 0;
    } else if (first_positive_index == 0 && second_positive_index == 0) {

        int count = 0;

        for (int i = eight_bit - 1; i >= 0; i--) {
            if (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 0 && count != 0) {
                summ_direct_result[i] = 1;
                count--;
            } else if ((first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 0 && count != 0) || (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 1 && count != 0)) {
                summ_direct_result[i] = 0;
                count--;
            } else if ((first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 1 && count != 0) || (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 0) || (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 1)) {
                summ_direct_result[i] = 1;
            } else if (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 0) {
                summ_direct_result[i] = 0;
            } else if (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 1) {
                summ_direct_result[i] = 0;
                count++;
            }
        }
        summ_direct_result[0] = 1;
    } else if ((first_positive_index == 1 && second_positive_index == 0) || (first_positive_index == 0 && second_positive_index == 1)) {
        if (number1 > number2) {
            summ_direct_result[0] = first_positive_index;
            for (int i = eight_bit - 1; i >= 0; i--) {
                if ((first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 0) || (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 1)) {
                    summ_direct_result[i] = 0;
                } else if (first_number_eight_bit_direct_sum[i] == 1 && second_number_eight_bit_direct_sum[i] == 0) {
                    summ_direct_result[i] = 1;
                } else if (first_number_eight_bit_direct_sum[i] == 0 && second_number_eight_bit_direct_sum[i] == 1) {
                    int j = i;
                    while (first_number_eight_bit_direct_sum[j] == 1) {
                        j--;
                        if (first_number_eight_bit_direct_sum[j] == 1) break;
                    }
                    first_number_eight_bit_direct_sum[i] = 2;
                    for (int g = j; g < i; g++) {
                        first_number_eight_bit_direct_sum[g] = 1;
                    }
                }
            }
        }else if (number1 < number2) {
            summ_direct_result[0] = second_positive_index;
            int bufer_second_number_eight_bit_direct_sum[eight_bit];
            for (int i = eight_bit - 1; i >= 0; i--) {
                bufer_second_number_eight_bit_direct_sum[i] = second_number_eight_bit_direct_sum[i];
                second_number_eight_bit_direct_sum[i] = first_number_eight_bit_direct_sum[i];
                first_number_eight_bit_direct_sum[i] = bufer_second_number_eight_bit_direct_sum[i];
            }

        } else if (number1 == number2) {
            for (int i = 0; i < eight_bit; i++) {
                summ_direct_result[0] = 0;
            }
        }
    }
}

void display_summ_direct(int eight_bit, int summ_direct_result[]) {
    cout << endl;
    wall();
    cout << "\nSummary between 1st and 2nd:         ";
    for (int i = 0; i < eight_bit; i++) {
        cout << summ_direct_result[i] << " ";
    }
    cout << endl;
    wall();
}

// void summ_back(int first_positive_index, int second_positive_index, int summ_back[], int first_number_eight_bit_back[], int second_number_eight_bit_back[]) {
//     if (first_positive_index == 1 && second_positive_index == 1) {
//
//     }else if (first_positive_index == 0 && second_positive_index == 0) {
//
//     }else if (first_positive_index == 1 && second_positive_index == 0) {
//
//     }else if (first_positive_index == 0 && second_positive_index == 1) {
//
//     }
// }
//
// void summ_additional(int first_positive_index, int second_positive_index, int summ_additional[], int first_number_eight_bit_additional[], int second_number_eight_bit_additional[]) {
//     if (first_positive_index == 1 && second_positive_index == 1) {
//
//     }else if (first_positive_index == 0 && second_positive_index == 0) {
//
//     }else if (first_positive_index == 1 && second_positive_index == 0) {
//
//     }else if (first_positive_index == 0 && second_positive_index == 1) {
//
//     }
// }

int main() {
    setlocale(LC_ALL, "ru");

    int number1, number2, first_positive_index, second_positive_index;
    int eight_bit = 8;

    enter_numbers(number1, number2, first_positive_index, second_positive_index);
    display_number(number1, first_positive_index, "1st");

    int first_bit_depth = calculate_bit_depth(number1);
    display_bit_depth(first_bit_depth, "1st");

    int first_number_bin[first_bit_depth];
    create_and_fill_binary(first_bit_depth, number1, first_number_bin);
    display_binary_form(first_bit_depth, first_number_bin, "1st");

    int first_number_eight_bit[eight_bit];
    create_and_fill_eight_bit(eight_bit, first_bit_depth, first_number_bin, first_number_eight_bit);
    display_eight_bit(eight_bit, first_number_eight_bit, "1st");

    int first_number_eight_bit_direct[eight_bit];
    int first_number_eight_bit_back[eight_bit];
    int first_number_eight_bit_additional[eight_bit];

    if (first_positive_index == 0) {
        create_and_fill_direct(eight_bit, first_number_eight_bit_direct, first_number_eight_bit);
        display_direct(eight_bit, first_number_eight_bit_direct, "1st");

        create_and_fill_back(first_number_eight_bit_back, eight_bit, first_number_eight_bit_direct);
        display_back(eight_bit, first_number_eight_bit_back, "1st");

        create_and_fill_additional(eight_bit, first_number_eight_bit_additional, first_number_eight_bit_back);
        display_additional(eight_bit, first_number_eight_bit_additional, "1st");
    } else if (first_positive_index == 1) {
        create_and_fill_positive_direct(eight_bit, first_number_eight_bit, first_number_eight_bit_direct);
        display_direct(eight_bit, first_number_eight_bit_direct, "1st");

        create_and_fill_positive_back(eight_bit, first_number_eight_bit, first_number_eight_bit_back);
        display_positive_back(eight_bit, first_number_eight_bit_back, "1st");

        create_and_fill_positive_additional(eight_bit, first_number_eight_bit, first_number_eight_bit_additional);
        display_additional(eight_bit, first_number_eight_bit_additional, "1st");
    }

    display_number(number2, second_positive_index, "2nd");

    int second_bit_depth = calculate_bit_depth(number2);
    display_bit_depth(second_bit_depth, "2nd");

    int second_number_bin[second_bit_depth];
    create_and_fill_binary(second_bit_depth, number2, second_number_bin);
    display_binary_form(second_bit_depth, second_number_bin, "2nd");

    int second_number_eight_bit[eight_bit];
    create_and_fill_eight_bit(eight_bit, second_bit_depth, second_number_bin, second_number_eight_bit);
    display_eight_bit(eight_bit, second_number_eight_bit, "2nd");

    int second_number_eight_bit_direct[eight_bit];
    int second_number_eight_bit_back[eight_bit];
    int second_number_eight_bit_additional[eight_bit];

    if (second_positive_index == 0) {
        create_and_fill_direct(eight_bit, second_number_eight_bit_direct, second_number_eight_bit);
        display_direct(eight_bit, second_number_eight_bit_direct, "2nd");

        create_and_fill_back(second_number_eight_bit_back, eight_bit, second_number_eight_bit_direct);
        display_back(eight_bit, second_number_eight_bit_back, "2nd");

        create_and_fill_additional(eight_bit, second_number_eight_bit_additional, second_number_eight_bit_back);
        display_additional(eight_bit, second_number_eight_bit_additional, "2nd");
    } else if (second_positive_index == 1) {
        create_and_fill_positive_direct(eight_bit, second_number_eight_bit, second_number_eight_bit_direct);
        display_direct(eight_bit, second_number_eight_bit_direct, "2nd");

        create_and_fill_positive_back(eight_bit, second_number_eight_bit, second_number_eight_bit_back);
        display_positive_back(eight_bit, second_number_eight_bit_back, "2nd");

        create_and_fill_positive_additional(eight_bit, second_number_eight_bit, second_number_eight_bit_additional);
        display_additional(eight_bit, second_number_eight_bit_additional, "2nd");
    }

    int first_number_eight_bit_direct_sum[eight_bit];
    int second_number_eight_bit_direct_sum[eight_bit];
    int summ_direct_result[eight_bit];

    summ_direct(number1, number2, first_positive_index, second_positive_index, eight_bit, summ_direct_result, first_number_eight_bit_direct, first_number_eight_bit_direct_sum, second_number_eight_bit_direct, second_number_eight_bit_direct_sum);
    display_direct(eight_bit, first_number_eight_bit_direct, "1st");
    display_direct(eight_bit, second_number_eight_bit_direct, "2nd");
    display_summ_direct(eight_bit, summ_direct_result);
}
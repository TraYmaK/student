#include <iostream>
#include <locale>
#include <string>

using namespace std;

constexpr int CODE_BITS = 8;

void major_wall() {
    cout << "==================================================" << endl;
}

void wall() {
    cout << "--------------------------------------------------" << endl;
}

int absolute_number(int number) {
    if (number < 0) {
        return -number;
    }
    return number;
}

int create_signed_number(int number_module, int positive_index) {
    if (positive_index == 1) {
        return number_module;
    }
    return -number_module;
}

int calculate_bit_depth(int number) {
    if (number <= 1) return 1;
    if (number <= 3) return 2;
    if (number <= 7) return 3;
    if (number <= 15) return 4;
    if (number <= 31) return 5;
    if (number <= 63) return 6;
    return 7;
}

void create_and_fill_binary(int bit_depth, int number, int number_bin[]) {
    int number_loop = number;

    for (int i = bit_depth - 1; i >= 0; i--) {
        number_bin[i] = number_loop % 2;
        number_loop = number_loop / 2;
    }
}

void display_bits_spaced(const int bits[], int len) {
    for (int i = 0; i < len; i++) {
        cout << bits[i];
        if (i != len - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

void display_natural_binary_non_negative(int value) {
    if (value == 0) {
        cout << "0" << endl;
        return;
    }
    int depth = calculate_bit_depth(value);
    int buf[32];
    for (int i = 0; i < depth; i++) {
        buf[i] = 0;
    }
    create_and_fill_binary(depth, value, buf);
    display_bits_spaced(buf, depth);
}

int index_of_highest_set_bit(unsigned x) {
    int k = -1;
    for (int i = 0; i < 32; i++) {
        if (((x >> i) & 1u) != 0) {
            k = i;
        }
    }
    return k;
}

unsigned long long add_u64_ripple(unsigned long long a, unsigned long long b) {
    unsigned long long sum = 0;
    unsigned long long carry = 0;
    for (int bit = 0; bit < 64; bit++) {
        unsigned long long ai = (a >> bit) & 1ULL;
        unsigned long long bi = (b >> bit) & 1ULL;
        unsigned long long si = ai ^ bi ^ carry;
        unsigned long long new_carry = (ai & bi) | (carry & (ai ^ bi));
        if (si != 0) {
            sum |= (1ULL << bit);
        }
        carry = new_carry;
    }
    return sum;
}

unsigned long long multiply_unsigned_shift_add(unsigned multiplicand, unsigned multiplier, bool print_steps) {
    unsigned long long product = 0;
    int hi = index_of_highest_set_bit(multiplier);
    if (hi < 0) {
        if (print_steps) {
            cout << "  Результат: 0" << endl;
        }
        return 0;
    }
    for (int i = 0; i <= hi; i++) {
        if (((multiplier >> i) & 1u) != 0) {
            unsigned long long summand = (unsigned long long)multiplicand << i;
            if (print_steps) {
                cout << "  Этап i=" << i << ": +" << summand << endl;
            }
            product = add_u64_ripple(product, summand);
        }
    }
    if (print_steps) {
        cout << "  Результат: " << product << endl;
    }
    return product;
}

unsigned divide_unsigned_binary(unsigned dividend, unsigned divisor, unsigned& remainder_out, bool print_steps) {
    remainder_out = 0;
    if (divisor == 0) {
        if (print_steps) {
            cout << "  Результат: недопустимо (делитель 0)" << endl;
        }
        return 0;
    }
    unsigned quotient = 0;
    unsigned long long remainder = 0;
    for (int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | (unsigned long long)((dividend >> i) & 1u);
        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= (1u << i);
        }
    }
    remainder_out = (unsigned)remainder;
    if (print_steps) {
        cout << "  Результат: q=" << quotient << " r=" << remainder_out << endl;
    }
    return quotient;
}

int order_exponent_from_bits(const int bits[], int n) {
    int value = 0;
    for (int i = 0; i < n; i++) {
        value = (value << 1) | bits[i];
    }
    return value;
}

void print_scaled_quotient_five_decimal(unsigned scaled_value) {
    unsigned rem = 0;
    unsigned int_part = divide_unsigned_binary(scaled_value, 100000u, rem, false);
    cout << int_part << ".";
    int frac = (int)rem;
    if (frac < 10000) {
        cout << "0";
    }
    if (frac < 1000) {
        cout << "0";
    }
    if (frac < 100) {
        cout << "0";
    }
    if (frac < 10) {
        cout << "0";
    }
    cout << frac;
}

void create_and_fill_eight_bit_module(int number, int number_eight_bit[]) {
    int bit_depth = calculate_bit_depth(number);
    int number_bin[CODE_BITS] = {0};

    create_and_fill_binary(bit_depth, number, number_bin);

    for (int i = 0; i < CODE_BITS; i++) {
        number_eight_bit[i] = 0;
    }

    for (int i = CODE_BITS - bit_depth, j = 0; i < CODE_BITS; i++, j++) {
        number_eight_bit[i] = number_bin[j];
    }
}

void create_and_fill_direct_code(int signed_number, int direct_code[]) {
    int number_module = absolute_number(signed_number);
    int number_eight_bit_module[CODE_BITS] = {0};

    create_and_fill_eight_bit_module(number_module, number_eight_bit_module);

    for (int i = 0; i < CODE_BITS; i++) {
        direct_code[i] = number_eight_bit_module[i];
    }

    if (signed_number < 0) {
        direct_code[0] = 1;
    } else {
        direct_code[0] = 0;
    }
}

void create_and_fill_back_code(int signed_number, int back_code[]) {
    int direct_code[CODE_BITS] = {0};
    create_and_fill_direct_code(signed_number, direct_code);

    if (signed_number >= 0) {
        for (int i = 0; i < CODE_BITS; i++) {
            back_code[i] = direct_code[i];
        }
    } else {
        back_code[0] = 1;
        for (int i = 1; i < CODE_BITS; i++) {
            if (direct_code[i] == 1) {
                back_code[i] = 0;
            } else {
                back_code[i] = 1;
            }
        }
    }
}

void create_and_fill_additional_code(int signed_number, int additional_code[]) {
    int back_code[CODE_BITS] = {0};
    create_and_fill_back_code(signed_number, back_code);

    if (signed_number >= 0) {
        for (int i = 0; i < CODE_BITS; i++) {
            additional_code[i] = back_code[i];
        }
    } else {
        for (int i = 0; i < CODE_BITS; i++) {
            additional_code[i] = back_code[i];
        }

        int carry = 1;

        for (int i = CODE_BITS - 1; i >= 0; i--) {
            int sum = additional_code[i] + carry;

            if (sum == 2) {
                additional_code[i] = 0;
                carry = 1;
            } else {
                additional_code[i] = sum;
                carry = 0;
                break;
            }
        }
    }
}

void display_code_array(const int code_array[]) {
    for (int i = 0; i < CODE_BITS; i++) {
        cout << code_array[i];
        if (i != CODE_BITS - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

void invert_sign_of_direct_code(int direct_code[]) {
    if (direct_code[0] == 0) {
        direct_code[0] = 1;
    } else {
        direct_code[0] = 0;
    }
}

void add_module_parts(const int first_code[], const int second_code[], int result_module[], int& carry_out) {
    int carry = 0;

    for (int i = CODE_BITS - 1; i >= 1; i--) {
        int sum = first_code[i] + second_code[i] + carry;

        if (sum == 0) {
            result_module[i] = 0;
            carry = 0;
        } else if (sum == 1) {
            result_module[i] = 1;
            carry = 0;
        } else if (sum == 2) {
            result_module[i] = 0;
            carry = 1;
        } else {
            result_module[i] = 1;
            carry = 1;
        }
    }
    carry_out = carry;
}

int compare_module_parts(const int first_code[], const int second_code[]) {
    for (int i = 1; i < CODE_BITS; i++) {
        if (first_code[i] > second_code[i]) {
            return 1;
        }
        if (first_code[i] < second_code[i]) {
            return -1;
        }
    }
    return 0;
}

void subtract_module_parts(const int bigger_code[], const int smaller_code[], int result_module[]) {
    int borrow = 0;

    for (int i = CODE_BITS - 1; i >= 1; i--) {
        int left_bit = bigger_code[i] - borrow;
        int right_bit = smaller_code[i];

        if (left_bit < right_bit) {
            result_module[i] = left_bit + 2 - right_bit;
            borrow = 1;
        } else {
            result_module[i] = left_bit - right_bit;
            borrow = 0;
        }
    }
}

void add_direct_code_arrays(const int first_direct_code[], const int second_direct_code[], int result_direct_code[]) {
    int first_sign = first_direct_code[0];
    int second_sign = second_direct_code[0];

    for (int i = 0; i < CODE_BITS; i++) {
        result_direct_code[i] = 0;
    }

    if (first_sign == second_sign) {
        int result_module[CODE_BITS] = {0};
        int carry_out = 0;
        add_module_parts(first_direct_code, second_direct_code, result_module, carry_out);

        result_direct_code[0] = first_sign;
        for (int i = 1; i < CODE_BITS; i++) {
            result_direct_code[i] = result_module[i];
        }
        if (carry_out == 1) {
            cout << "  Переполнение модуля (прямой код)" << endl;
        }
    } else {
        int compare_result = compare_module_parts(first_direct_code, second_direct_code);

        if (compare_result == 0) {
            for (int i = 0; i < CODE_BITS; i++) {
                result_direct_code[i] = 0;
            }
        } else if (compare_result == 1) {
            int result_module[CODE_BITS] = {0};
            subtract_module_parts(first_direct_code, second_direct_code, result_module);

            result_direct_code[0] = first_sign;
            for (int i = 1; i < CODE_BITS; i++) {
                result_direct_code[i] = result_module[i];
            }
        } else {
            int result_module[CODE_BITS] = {0};
            subtract_module_parts(second_direct_code, first_direct_code, result_module);

            result_direct_code[0] = second_sign;
            for (int i = 1; i < CODE_BITS; i++) {
                result_direct_code[i] = result_module[i];
            }
        }
    }
}

void subtract_direct_code_arrays(const int first_direct_code[], const int second_direct_code[], int result_direct_code[]) {
    int changed_second_direct_code[CODE_BITS] = {0};

    for (int i = 0; i < CODE_BITS; i++) {
        changed_second_direct_code[i] = second_direct_code[i];
    }
    invert_sign_of_direct_code(changed_second_direct_code);
    add_direct_code_arrays(first_direct_code, changed_second_direct_code, result_direct_code);
}

void add_back_code_arrays(const int first_back_code[], const int second_back_code[], int result_back_code[]) {
    int carry = 0;

    for (int i = CODE_BITS - 1; i >= 0; i--) {
        int sum = first_back_code[i] + second_back_code[i] + carry;

        if (sum == 0) {
            result_back_code[i] = 0;
            carry = 0;
        } else if (sum == 1) {
            result_back_code[i] = 1;
            carry = 0;
        } else if (sum == 2) {
            result_back_code[i] = 0;
            carry = 1;
        } else {
            result_back_code[i] = 1;
            carry = 1;
        }
    }

    if (carry == 1) {
        for (int i = CODE_BITS - 1; i >= 0; i--) {
            int sum = result_back_code[i] + carry;

            if (sum == 0) {
                result_back_code[i] = 0;
                carry = 0;
            } else if (sum == 1) {
                result_back_code[i] = 1;
                carry = 0;
                break;
            } else if (sum == 2) {
                result_back_code[i] = 0;
                carry = 1;
            }
        }
    }
}

void subtract_back_code_arrays(int first_signed_number, int second_signed_number, int result_back_code[]) {
    int first_back_code[CODE_BITS] = {0};
    int changed_second_back_code[CODE_BITS] = {0};

    create_and_fill_back_code(first_signed_number, first_back_code);
    create_and_fill_back_code(-second_signed_number, changed_second_back_code);

    add_back_code_arrays(first_back_code, changed_second_back_code, result_back_code);
}

void add_additional_code_arrays(const int first_additional_code[], const int second_additional_code[], int result_additional_code[]) {
    int carry = 0;

    for (int i = CODE_BITS - 1; i >= 0; i--) {
        int sum = first_additional_code[i] + second_additional_code[i] + carry;

        if (sum == 0) {
            result_additional_code[i] = 0;
            carry = 0;
        } else if (sum == 1) {
            result_additional_code[i] = 1;
            carry = 0;
        } else if (sum == 2) {
            result_additional_code[i] = 0;
            carry = 1;
        } else {
            result_additional_code[i] = 1;
            carry = 1;
        }
    }
}

void subtract_additional_code_arrays(int first_signed_number, int second_signed_number, int result_additional_code[]) {
    int first_additional_code[CODE_BITS] = {0};
    int changed_second_additional_code[CODE_BITS] = {0};

    create_and_fill_additional_code(first_signed_number, first_additional_code);
    create_and_fill_additional_code(-second_signed_number, changed_second_additional_code);

    add_additional_code_arrays(first_additional_code, changed_second_additional_code, result_additional_code);
}

void display_number_codes_compact(int number, const string& number_name) {
    int direct_code[CODE_BITS] = {0};
    int back_code[CODE_BITS] = {0};
    int additional_code[CODE_BITS] = {0};

    create_and_fill_direct_code(number, direct_code);
    create_and_fill_back_code(number, back_code);
    create_and_fill_additional_code(number, additional_code);

    int module = absolute_number(number);
    cout << number_name << " = " << number << endl;
    cout << "  |" << number_name << "|₂: ";
    display_natural_binary_non_negative(module);
    cout << "  Прямой: ";
    display_code_array(direct_code);
    cout << "  Обратный: ";
    display_code_array(back_code);
    cout << "  Дополнительный: ";
    display_code_array(additional_code);
}

void display_operation_result_codes_compact(const string& operation_title, int decimal_result, const int result_direct_code[], const int result_back_code[], const int result_additional_code[]) {
    cout << operation_title << endl;
    cout << "  Результат (10): " << decimal_result << endl;
    int abs_res = absolute_number(decimal_result);
    cout << "  |рез|₂: ";
    display_natural_binary_non_negative(abs_res);
    cout << "  Прямой: ";
    display_code_array(result_direct_code);
    cout << "  Обратный: ";
    display_code_array(result_back_code);
    cout << "  Дополнительный: ";
    display_code_array(result_additional_code);
}

void display_operation_results_in_all_codes(int first_number, int second_number, const string& operation_name) {
    int first_direct_code[CODE_BITS] = {0};
    int second_direct_code[CODE_BITS] = {0};
    int first_back_code[CODE_BITS] = {0};
    int second_back_code[CODE_BITS] = {0};
    int first_additional_code[CODE_BITS] = {0};
    int second_additional_code[CODE_BITS] = {0};

    int result_direct_code[CODE_BITS] = {0};
    int result_back_code[CODE_BITS] = {0};
    int result_additional_code[CODE_BITS] = {0};

    create_and_fill_direct_code(first_number, first_direct_code);
    create_and_fill_direct_code(second_number, second_direct_code);
    create_and_fill_back_code(first_number, first_back_code);
    create_and_fill_back_code(second_number, second_back_code);
    create_and_fill_additional_code(first_number, first_additional_code);
    create_and_fill_additional_code(second_number, second_additional_code);

    if (operation_name == "X1+X2") {
        add_direct_code_arrays(first_direct_code, second_direct_code, result_direct_code);
        add_back_code_arrays(first_back_code, second_back_code, result_back_code);
        add_additional_code_arrays(first_additional_code, second_additional_code, result_additional_code);

        display_operation_result_codes_compact("X1+X2", first_number + second_number, result_direct_code, result_back_code, result_additional_code);
    } else {
        subtract_direct_code_arrays(first_direct_code, second_direct_code, result_direct_code);
        subtract_back_code_arrays(first_number, second_number, result_back_code);
        subtract_additional_code_arrays(first_number, second_number, result_additional_code);

        display_operation_result_codes_compact("X1-X2", first_number - second_number, result_direct_code, result_back_code, result_additional_code);
    }
}

void perform_first_task_for_one_sign_combination(int first_number_module, int second_number_module, int first_positive_index, int second_positive_index, const string& combination_name) {
    int first_number = create_signed_number(first_number_module, first_positive_index);
    int second_number = create_signed_number(second_number_module, second_positive_index);

    wall();
    cout << "Знаки: " << combination_name << endl;
    wall();
    cout << endl;

    cout << "Вход" << endl;
    display_number_codes_compact(first_number, "X1");
    cout << endl;
    display_number_codes_compact(second_number, "X2");
    cout << endl;

    display_operation_results_in_all_codes(first_number, second_number, "X1+X2");
    cout << endl;
    display_operation_results_in_all_codes(first_number, second_number, "X1-X2");
    cout << endl;
}

void perform_first_task(int variant_number, int x1, int x2) {
    major_wall();
    cout << "ЗАДАНИЕ 1. Сложение и вычитание в кодах" << endl;
    cout << "Вариант " << variant_number << ", |X1|=" << x1 << " |X2|=" << x2 << ", сетка " << CODE_BITS << " бит" << endl;
    major_wall();
    cout << endl;

    perform_first_task_for_one_sign_combination(x1, x2, 1, 1, "+/+");
    perform_first_task_for_one_sign_combination(x1, x2, 1, 0, "+/-");
    perform_first_task_for_one_sign_combination(x1, x2, 0, 1, "-/+");
    perform_first_task_for_one_sign_combination(x1, x2, 0, 0, "-/-");
}

void mantissa_bits_from_module_array(int module, int bits[], int& len) {
    len = calculate_bit_depth(module);
    create_and_fill_binary(len, module, bits);
}

void print_fraction_line_array(const int bits[], int len) {
    cout << "0.";
    for (int i = 0; i < len; i++) {
        cout << bits[i];
    }
}

void align_mantissa_right_array(const int bits[], int len, int shift, int out[], int& out_len) {
    out_len = len + shift;
    for (int i = 0; i < shift; i++) {
        out[i] = 0;
    }
    for (int j = 0; j < len; j++) {
        out[shift + j] = bits[j];
    }
}

void add_binary_fraction_arrays(const int a[], int na, const int b[], int nb, int sum[], int& nsum, int& carry_out) {
    int n = na;
    if (nb > n) {
        n = nb;
    }
    int aa[32];
    int bb[32];
    int pad_a = n - na;
    int pad_b = n - nb;
    for (int i = 0; i < n; i++) {
        aa[i] = (i < pad_a) ? 0 : a[i - pad_a];
        bb[i] = (i < pad_b) ? 0 : b[i - pad_b];
    }
    int carry = 0;
    for (int i = n - 1; i >= 0; i--) {
        int s = aa[i] + bb[i] + carry;
        sum[i] = s % 2;
        carry = s / 2;
    }
    carry_out = carry;
    nsum = n;
}

void strip_leading_zeros_array(int bits[], int& len) {
    while (len > 1 && bits[0] == 0) {
        for (int i = 0; i < len - 1; i++) {
            bits[i] = bits[i + 1];
        }
        len--;
    }
}

void perform_fourth_task(int x1, int x2) {
    const int p1_bits[] = {1, 0, 0};
    const int p2_bits[] = {1, 0, 1};
    const int p1_field_len = 3;
    const int p2_field_len = 3;
    const int p1 = order_exponent_from_bits(p1_bits, p1_field_len);
    const int p2 = order_exponent_from_bits(p2_bits, p2_field_len);

    int m1 = absolute_number(x1);
    int m2 = absolute_number(x2);

    int mantissa1_bits[CODE_BITS];
    int mantissa2_bits[CODE_BITS];
    int len1 = 0;
    int len2 = 0;
    mantissa_bits_from_module_array(m1, mantissa1_bits, len1);
    mantissa_bits_from_module_array(m2, mantissa2_bits, len2);

    major_wall();
    cout << "ЗАДАНИЕ 4. Сложение с плавающей точкой" << endl;
    major_wall();
    cout << endl;

    cout << "P1 (0,100): ";
    for (int i = 0; i < p1_field_len; i++) {
        cout << p1_bits[i];
        if (i + 1 < p1_field_len) {
            cout << " ";
        }
    }
    cout << " -> " << p1 << endl;
    cout << "P2 (0,101): ";
    for (int i = 0; i < p2_field_len; i++) {
        cout << p2_bits[i];
        if (i + 1 < p2_field_len) {
            cout << " ";
        }
    }
    cout << " -> " << p2 << endl;
    cout << endl;

    cout << "Исходные" << endl;
    cout << "  X1 = ";
    print_fraction_line_array(mantissa1_bits, len1);
    cout << " * 2^" << p1 << " = " << m1 << endl;
    cout << "  X2 = ";
    print_fraction_line_array(mantissa2_bits, len2);
    cout << " * 2^" << p2 << " = " << m2 << endl;
    cout << endl;

    int p_max = p1;
    if (p2 > p_max) {
        p_max = p2;
    }
    int shift1 = p_max - p1;
    int shift2 = p_max - p2;

    int aligned1[32];
    int aligned2[32];
    int n1 = 0;
    int n2 = 0;
    align_mantissa_right_array(mantissa1_bits, len1, shift1, aligned1, n1);
    align_mantissa_right_array(mantissa2_bits, len2, shift2, aligned2, n2);

    cout << "Этап 1. Выравнивание порядков, P_max=" << p_max << endl;
    cout << "  X1 -> ";
    print_fraction_line_array(aligned1, n1);
    cout << " * 2^" << p_max << endl;
    cout << "  X2 -> ";
    print_fraction_line_array(aligned2, n2);
    cout << " * 2^" << p_max << endl;
    cout << endl;

    int sum_bits[32];
    int nsum = 0;
    int carry_sum = 0;
    add_binary_fraction_arrays(aligned1, n1, aligned2, n2, sum_bits, nsum, carry_sum);

    cout << "Этап 2. Сложение мантисс, P=" << p_max << endl;
    cout << "       ";
    print_fraction_line_array(aligned1, n1);
    cout << endl;
    cout << "     + ";
    print_fraction_line_array(aligned2, n2);
    cout << endl;
    cout << "     = ";
    if (carry_sum == 1) {
        cout << "1.";
    } else {
        cout << "0.";
    }
    for (int i = 0; i < nsum; i++) {
        cout << sum_bits[i];
    }
    cout << endl;
    cout << endl;

    int result_bits[32];
    int result_len = 0;
    int result_p = p_max;
    if (carry_sum == 1) {
        result_bits[0] = 1;
        for (int i = 0; i < nsum; i++) {
            result_bits[i + 1] = sum_bits[i];
        }
        result_len = nsum + 1;
        result_p = p_max + 1;
    } else {
        for (int i = 0; i < nsum; i++) {
            result_bits[i] = sum_bits[i];
        }
        result_len = nsum;
    }
    strip_leading_zeros_array(result_bits, result_len);

    cout << "Этап 3. Результат (нормализация)" << endl;
    cout << "  ";
    print_fraction_line_array(result_bits, result_len);
    cout << " * 2^" << result_p << endl;
    cout << "  Сумма (10): " << (m1 + m2) << endl;
    cout << endl;
}

void run_module_division_algorithm_once(unsigned x1_module, unsigned x2_module, unsigned& scaled_modules_out) {
    cout << endl;
    cout << "Деление |X1|/|X2|" << endl;
    cout << "Этап 1. Целое деление (двоичное)" << endl;

    unsigned rem_int = 0;
    unsigned q_int = divide_unsigned_binary(x1_module, x2_module, rem_int, true);

    cout << "Этап 2. Дробная часть (остаток×10, цифра, новый остаток)" << endl;
    unsigned r = rem_int;
    int digits[6];
    for (int k = 0; k < 6; k++) {
        unsigned mul10 = (unsigned)multiply_unsigned_shift_add(r, 10u, false);
        unsigned digit = divide_unsigned_binary(mul10, x2_module, r, false);
        digits[k] = (int)digit;
        if (k < 5) {
            cout << "  " << (k + 1) << ": r*10=" << mul10 << " цифра=" << digit << " r'=" << r << endl;
        } else {
            cout << "  6: r*10=" << mul10 << " цифра=" << digit << " r'=" << r << " (округление)" << endl;
        }
    }

    unsigned frac_scaled = 0;
    for (int k = 0; k < 5; k++) {
        frac_scaled = (unsigned)multiply_unsigned_shift_add(frac_scaled, 10u, false) + (unsigned)digits[k];
    }
    if (digits[5] >= 5) {
        frac_scaled = frac_scaled + 1;
    }
    while (frac_scaled >= 100000u) {
        frac_scaled = frac_scaled - 100000u;
        q_int = q_int + 1;
    }

    unsigned int_scaled = (unsigned)multiply_unsigned_shift_add(q_int, 100000u, false);
    scaled_modules_out = int_scaled + frac_scaled;
    cout << "  Результат ×1e5: " << scaled_modules_out << endl;
    cout << endl;
}

void display_multiplication_combo(int first_positive_index, int second_positive_index, int x1, int x2, unsigned long long module_product) {
    int first_number = create_signed_number(x1, first_positive_index);
    int second_number = create_signed_number(x2, second_positive_index);
    bool neg = (first_number < 0) != (second_number < 0);
    long long signed_prod = (long long)module_product;
    if (neg) {
        signed_prod = -signed_prod;
    }

    char s1 = first_number >= 0 ? '+' : '-';
    char s2 = second_number >= 0 ? '+' : '-';
    cout << "Итог X1*X2: " << first_number << " * " << second_number << " = " << signed_prod;
    cout << "  (знаки " << s1 << "/" << s2 << ", |X1|·|X2| = " << module_product << ")" << endl;
    cout << endl;
}

void display_division_combo(int first_positive_index, int second_positive_index, int x1, int x2, unsigned scaled_modules) {
    int first_number = create_signed_number(x1, first_positive_index);
    int second_number = create_signed_number(x2, second_positive_index);

    if (x2 == 0) {
        cout << "Результат: делитель 0" << endl;
        cout << endl;
        return;
    }

    bool neg = (first_number < 0) != (second_number < 0);
    char s1 = first_number >= 0 ? '+' : '-';
    char s2 = second_number >= 0 ? '+' : '-';

    cout << "X1/X2 = " << first_number << " / " << second_number << ", знаки " << s1 << " / " << s2 << " -> ";
    if (neg) {
        cout << "-";
    }
    print_scaled_quotient_five_decimal(scaled_modules);
    cout << "  (|X1|/|X2|, 5 знаков; |X1|/|X2|×1e5=" << scaled_modules << ")" << endl;
    cout << endl;
}

void perform_second_task(int x1, int x2) {
    major_wall();
    cout << "ЗАДАНИЕ 2. Умножение" << endl;
    major_wall();
    cout << endl;

    cout << "|X1|₂: ";
    display_natural_binary_non_negative(x1);
    cout << "|X2|₂: ";
    display_natural_binary_non_negative(x2);
    cout << "Умножение модулей" << endl;
    unsigned long long module_product = multiply_unsigned_shift_add((unsigned)x1, (unsigned)x2, true);
    cout << "|X1|·|X2|₂: ";
    display_natural_binary_non_negative((int)module_product);
    cout << endl;

    display_multiplication_combo(1, 1, x1, x2, module_product);
    display_multiplication_combo(1, 0, x1, x2, module_product);
    display_multiplication_combo(0, 1, x1, x2, module_product);
    display_multiplication_combo(0, 0, x1, x2, module_product);

    cout << endl;
}

void perform_third_task(int x1, int x2) {
    major_wall();
    cout << "ЗАДАНИЕ 3. Деление" << endl;
    major_wall();
    cout << endl;

    if (x2 == 0) {
        cout << "Результат: делитель 0" << endl;
        cout << endl;
        return;
    }

    cout << "|X1|₂: ";
    display_natural_binary_non_negative(x1);
    cout << "|X2|₂: ";
    display_natural_binary_non_negative(x2);

    unsigned scaled_modules = 0;
    run_module_division_algorithm_once((unsigned)x1, (unsigned)x2, scaled_modules);

    display_division_combo(1, 1, x1, x2, scaled_modules);
    display_division_combo(1, 0, x1, x2, scaled_modules);
    display_division_combo(0, 1, x1, x2, scaled_modules);
    display_division_combo(0, 0, x1, x2, scaled_modules);

    cout << endl;
}

int main() {
    setlocale(LC_ALL, "ru");

    const int variant_number = 10;
    const int x1 = 11;
    const int x2 = 21;
    major_wall();
    cout << "Лабораторная работа 1" << endl;
    cout << "Вариант " << variant_number << ", |X1|=" << x1 << " |X2|=" << x2 << ", коды " << CODE_BITS << " бит" << endl;
    major_wall();
    cout << endl;

    perform_first_task(variant_number, x1, x2);
    perform_second_task(x1, x2);
    perform_third_task(x1, x2);
    perform_fourth_task(x1, x2);

    return 0;
}

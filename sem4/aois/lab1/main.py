import sys

BIT_COUNT = 8
VALUE_BITS = BIT_COUNT - 1

VARIANT = 24
DEFAULT_X1 = 13
DEFAULT_X2 = 26

ORDER_1 = 4
ORDER_2 = 5


def make_equal_length(a, b):
    size = max(len(a), len(b))
    return a.zfill(size), b.zfill(size)


def add_bits(a, b):
    a, b = make_equal_length(a, b)

    carry = 0
    answer = ""

    for i in range(len(a) - 1, -1, -1):
        current_sum = int(a[i]) + int(b[i]) + carry
        answer = str(current_sum % 2) + answer
        carry = current_sum // 2

    return answer, carry


def subtract_bits(a, b):
    result = ""
    borrow = 0

    for i in range(len(a) - 1, -1, -1):
        difference = int(a[i]) - int(b[i]) - borrow

        if difference < 0:
            difference += 2
            borrow = 1
        else:
            borrow = 0

        result = str(difference) + result

    return result


def to_direct_code(number):
    sign = "0" if number >= 0 else "1"
    value = abs(number)

    if value == 0:
        binary_value = "0"
    else:
        binary_value = ""

        while value > 0:
            binary_value = str(value % 2) + binary_value
            value //= 2

    binary_value = binary_value.zfill(VALUE_BITS)

    return sign + binary_value


def to_reverse_code(direct_code):
    sign = direct_code[0]

    if sign == "0":
        return direct_code

    result = sign

    for bit in direct_code[1:]:
        result += "1" if bit == "0" else "0"

    return result


def to_additional_code(direct_code):
    reverse_code = to_reverse_code(direct_code)

    if reverse_code[0] == "0":
        return reverse_code

    sign = reverse_code[0]
    value = reverse_code[1:]

    value, carry = add_bits(value, "1")

    if len(value) > VALUE_BITS:
        value = value[-VALUE_BITS:]

    return sign + value


def add_direct_codes(first_code, second_code):
    first_sign = first_code[0]
    second_sign = second_code[0]

    first_value = first_code[1:]
    second_value = second_code[1:]

    if first_sign == second_sign:
        result_value, carry = add_bits(first_value, second_value)

        if carry == 1 or len(result_value) > VALUE_BITS:
            return "Переполнение"

        return first_sign + result_value

    if first_value == second_value:
        return "0" + "0" * VALUE_BITS

    if first_value > second_value:
        result_value = subtract_bits(first_value, second_value)
        return first_sign + result_value

    result_value = subtract_bits(second_value, first_value)
    return second_sign + result_value


def add_reverse_codes(first_code, second_code):
    result, carry = add_bits(first_code, second_code)

    if carry == 1:
        result, extra_carry = add_bits(result, "1")

        if extra_carry == 1:
            result = result[-BIT_COUNT:]

    return result[-BIT_COUNT:]


def add_additional_codes(first_code, second_code):
    result, carry = add_bits(first_code, second_code)
    return result[-BIT_COUNT:]


def multiply_modules(first_code, second_code):
    first_module = first_code[1:]
    second_module = second_code[1:]

    result = "0"

    for shift, bit in enumerate(reversed(second_module)):
        if bit == "1":
            shifted_value = first_module + "0" * shift
            result, carry = add_bits(result, shifted_value)

            if carry == 1:
                result = "1" + result

    sign = "+" if first_code[0] == second_code[0] else "-"

    return sign, result


def divide_modules(first_code, second_code, precision=5):
    first_value = int(first_code[1:], 2)
    second_value = int(second_code[1:], 2)

    sign = "+" if first_code[0] == second_code[0] else "-"

    if second_value == 0:
        return sign, "Деление на ноль невозможно"

    integer_part = first_value // second_value
    remainder = first_value % second_value

    integer_binary = bin(integer_part)[2:]

    fraction_binary = ""

    for _ in range(precision):
        remainder *= 2

        if remainder >= second_value:
            fraction_binary += "1"
            remainder -= second_value
        else:
            fraction_binary += "0"

    return sign, integer_binary + "." + fraction_binary


def add_float_values(first_direct_code, second_direct_code):
    first_mantissa = first_direct_code[1:]
    second_mantissa = second_direct_code[1:]

    print(f"Порядки: P1 = {ORDER_1}, P2 = {ORDER_2}")
    print(f"Мантиссы: M1 = 0.{first_mantissa}, M2 = 0.{second_mantissa}")

    difference = ORDER_2 - ORDER_1

    if difference > 0:
        shifted_first = "0" * difference + first_mantissa[:-difference]
        shifted_second = second_mantissa
        result_order = ORDER_2
        print(f"Выравнивание: M1 -> 0.{shifted_first}")
    elif difference < 0:
        difference = abs(difference)
        shifted_first = first_mantissa
        shifted_second = "0" * difference + second_mantissa[:-difference]
        result_order = ORDER_1
        print(f"Выравнивание: M2 -> 0.{shifted_second}")
    else:
        shifted_first = first_mantissa
        shifted_second = second_mantissa
        result_order = ORDER_1
        print("Выравнивание не требуется")

    result_mantissa, carry = add_bits(shifted_first, shifted_second)

    if carry == 1:
        result_mantissa = result_mantissa[-VALUE_BITS:]

    return f"0.{result_mantissa} * 2^{result_order}"


def print_code_operation(first_number, second_number, title):
    print(f"\n--- {title}: {first_number} и {second_number} ---")

    first_direct = to_direct_code(first_number)
    second_direct = to_direct_code(second_number)

    direct_result = add_direct_codes(first_direct, second_direct)

    first_reverse = to_reverse_code(first_direct)
    second_reverse = to_reverse_code(second_direct)
    reverse_result = add_reverse_codes(first_reverse, second_reverse)

    first_additional = to_additional_code(first_direct)
    second_additional = to_additional_code(second_direct)
    additional_result = add_additional_codes(first_additional, second_additional)

    print(f"Первое число в ПК:       {first_direct}")
    print(f"Второе число в ПК:       {second_direct}")
    print(f"Результат в ПК:          {direct_result}")

    print(f"Первое число в ОК:       {first_reverse}")
    print(f"Второе число в ОК:       {second_reverse}")
    print(f"Результат в ОК:          {reverse_result}")

    print(f"Первое число в ДК:       {first_additional}")
    print(f"Второе число в ДК:       {second_additional}")
    print(f"Результат в ДК:          {additional_result}")


def print_sign_table_for_multiplication():
    print("\nТаблица знаков произведения:")

    signs = [
        ("+", "+", "+"),
        ("+", "-", "-"),
        ("-", "+", "-"),
        ("-", "-", "+")
    ]

    for first_sign, second_sign, result_sign in signs:
        print(f"{first_sign}|X1| * {second_sign}|X2| -> {result_sign}")


def print_sign_table_for_division():
    print("\nТаблица знаков частного:")

    signs = [
        ("+", "+", "+"),
        ("+", "-", "-"),
        ("-", "+", "-"),
        ("-", "-", "+")
    ]

    for first_sign, second_sign, result_sign in signs:
        print(f"{first_sign}|X1| / {second_sign}|X2| -> {result_sign}")


def main():
    print("ЛАБОРАТОРНАЯ РАБОТА №1")
    print(f"Вариант: {VARIANT}")
    print(f"X1 = {DEFAULT_X1}")
    print(f"X2 = {DEFAULT_X2}")

    try:
        x1 = int(input("\nВведите X1 или нажмите Enter для значения варианта: ") or DEFAULT_X1)
        x2 = int(input("Введите X2 или нажмите Enter для значения варианта: ") or DEFAULT_X2)
    except ValueError:
        print("Ошибка: нужно ввести целое число.")
        sys.exit()

    print("\n" + "=" * 60)
    print("ЗАДАНИЕ 1. Сложение в прямом, обратном и дополнительном кодах")
    print("=" * 60)

    operations = [
        (x1, x2, "(+X1) + (+X2)"),
        (x1, -x2, "(+X1) + (-X2)"),
        (-x1, x2, "(-X1) + (+X2)"),
        (-x1, -x2, "(-X1) + (-X2)")
    ]

    for first_number, second_number, title in operations:
        print_code_operation(first_number, second_number, title)

    first_base_code = to_direct_code(x1)
    second_base_code = to_direct_code(x2)

    print("\n" + "=" * 60)
    print("ЗАДАНИЕ 2. Умножение модулей")
    print("=" * 60)

    multiplication_sign, multiplication_result = multiply_modules(first_base_code, second_base_code)

    clean_multiplication_result = multiplication_result.lstrip("0") or "0"

    print(f"|X1| * |X2| в двоичном виде: {clean_multiplication_result}")
    print(f"|X1| * |X2| в десятичном виде: {int(clean_multiplication_result, 2)}")

    print_sign_table_for_multiplication()

    print("\n" + "=" * 60)
    print("ЗАДАНИЕ 3. Деление модулей")
    print("=" * 60)

    division_sign, division_result = divide_modules(first_base_code, second_base_code)

    print(f"|X1| / |X2| в двоичном виде: {division_result}")
    print("Точность дробной части: 5 разрядов")

    print_sign_table_for_division()

    print("\n" + "=" * 60)
    print("ЗАДАНИЕ 4. Сложение чисел с плавающей точкой")
    print("=" * 60)

    float_result = add_float_values(first_base_code, second_base_code)

    print(f"Результат: {float_result}")


if __name__ == "__main__":
    main()
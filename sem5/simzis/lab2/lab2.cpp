#include <cstddef>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

namespace {

const string kAlphabet = "abcdefghijklmnopqrstuvwxyz";

bool isAlphabetSymbol(char symbol) {
    return symbol >= 'a' && symbol <= 'z';
}

bool isValidText(const string& text) {
    if (text.empty()) {
        return false;
    }

    for (char symbol : text) {
        if (!isAlphabetSymbol(symbol)) {
            return false;
        }
    }
    return true;
}

bool isValidKey(const string& key) {
    return isValidText(key);
}

string readText(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!cin) {
            return {};
        }
        if (isValidText(value)) {
            return value;
        }
        cout << "Используйте только строчные буквы английского алфавита.\n";
    }
}

string readKey() {
    string key;
    while (true) {
        cout << "Введите ключ: ";
        getline(cin, key);
        if (!cin) {
            return {};
        }
        if (isValidKey(key)) {
            return key;
        }
        cout << "Ключ должен содержать только строчные буквы английского алфавита.\n";
    }
}

size_t readKeyLength() {
    size_t length = 0;
    while (true) {
        cout << "Введите длину ключа: ";
        if (cin >> length && length > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return length;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Длина ключа должна быть положительным числом.\n";
    }
}

string transform(
    const string& text,
    const string& key,
    bool encrypt) {
    string result = text;

    for (size_t index = 0; index < text.size(); ++index) {
        const int textValue = text[index] - 'a';
        const int keyValue = key[index % key.size()] - 'a';
        const int value = encrypt
            ? (textValue + keyValue) % static_cast<int>(kAlphabet.size())
            : (textValue - keyValue + static_cast<int>(kAlphabet.size())) %
                  static_cast<int>(kAlphabet.size());
        result[index] = kAlphabet[static_cast<size_t>(value)];
    }

    return result;
}

void searchKey(
    const string& encryptedText,
    const string& originalText,
    string& candidate,
    size_t position,
    size_t& attempts,
    bool& found) {
    if (position == candidate.size()) {
        ++attempts;
        if (transform(encryptedText, candidate, false) == originalText) {
            cout << "Найден ключ: " << candidate << "\n";
            found = true;
        }
        return;
    }

    for (char symbol : kAlphabet) {
        candidate[position] = symbol;
        searchKey(
            encryptedText,
            originalText,
            candidate,
            position + 1,
            attempts,
            found);
    }
}

void encryptText() {
    const string text = readText("Введите открытый текст: ");
    if (text.empty()) {
        return;
    }
    const string key = readKey();
    if (key.empty()) {
        return;
    }
    cout << "Зашифрованный текст: " << transform(text, key, true) << "\n";
}

void decryptText() {
    const string text = readText("Введите зашифрованный текст: ");
    if (text.empty()) {
        return;
    }
    const string key = readKey();
    if (key.empty()) {
        return;
    }
    cout << "Расшифрованный текст: " << transform(text, key, false) << "\n";
}

void bruteForceAttack() {
    const string encryptedText =
        readText("Введите зашифрованный текст: ");
    if (encryptedText.empty()) {
        return;
    }

    const string originalText =
        readText("Введите исходный открытый текст: ");
    if (originalText.empty()) {
        return;
    }

    if (encryptedText.size() != originalText.size()) {
        cout << "Длины текстов должны совпадать.\n";
        return;
    }

    const size_t keyLength = readKeyLength();
    string candidate(keyLength, kAlphabet[0]);
    size_t attempts = 0;
    bool found = false;

    searchKey(
        encryptedText,
        originalText,
        candidate,
        0,
        attempts,
        found);

    cout << "Проверено ключей: " << attempts << "\n";
    if (!found) {
        cout << "Подходящий ключ не найден.\n";
    }
}

int readAction() {
    int action = 0;
    while (true) {
        cout << "\n1. Зашифровать текст\n"
                "2. Расшифровать текст\n"
                "3. Выполнить атаку полным перебором ключа\n"
                "4. Выход\n"
                "Выберите действие: ";
        if (cin >> action &&
            action >= 1 &&
            action <= 4) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return action;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите число от 1 до 4.\n";
    }
}

}

int main() {
    cout << "Лабораторная работа №2. Шифр Виженера\n"
         << "Вариант 26. Вариант 2 из 4.\n"
         << "Используется английский алфавит.\n";

    while (true) {
        switch (readAction()) {
            case 1:
                encryptText();
                break;
            case 2:
                decryptText();
                break;
            case 3:
                bruteForceAttack();
                break;
            case 4:
                return 0;
        }
    }
}

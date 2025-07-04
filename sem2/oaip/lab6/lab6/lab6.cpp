#include <iostream>
#include "func.hpp"

using namespace std;

int main() {
	setlocale(LC_ALL, "ru");
	Element* root = nullptr;
	while (true) {
		wall();
		cout << "1. Создать дерево\n";
		cout << "2. Добавить элемент в дерево\n";
		cout << "3. Прямой обход дерева\n";
		cout << "4. Симметричный обход дерева\n";
		cout << "5. Обратный обход дерева\n";
		cout << "6. Балансировка дерева\n";
		cout << "7. Подсчёт кол-ва записей в дереве, начинающихся с заданной буквы\n";
		cout << "8. Найти элемент по ключу\n";
		cout << "9. Удалить элемент по ключу\n";
		cout << "10. Выход\n";
		int operation = 0;
		wall();
		cout << "Выберите операцию: ";
		cin >> operation;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			wall();
			cout << "Ошибка: введите номер операции (целое число).\n";
			wall();
			clear();
			continue;
		}

		if (operation < 1 || operation > 10) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			wall();
			cout << "Ошибка: операция вне допустимого диапазона (1–10).\n";
			wall();
			clear();
			continue;
		}

			if (operation == 1) {
				wall();
				clear();
				root = create_tree();
			}
			else if (operation == 2) {
				wall();
				clear();
				root = add_element(root);
			}
			else if (operation == 3) {
				wall();
				clear();
				wall();
				preorder(root);
				wall();
			}
			else if (operation == 4) {
				wall();
				clear();
				wall();
				inorder(root);
				wall();
			}
			else if (operation == 5) {
				wall();
				clear();
				wall();
				postorder(root);
				wall();
			}
			else if (operation == 6) {
				wall();
				clear();
				root = balance_tree(root);
				wall();
				cout << "Дерево сбалансировано.\n";
				wall();
			}
			else if (operation == 7) {
				wall();
				clear();
				char letter;
				wall();
				cout << "Введите первую букву для поиска: ";
				while (!(cin >> letter) || !isalpha(letter)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					wall();
					cout << "Ошибка: введите одну букву латинского или русского алфавита: ";
				}
				wall();
				int count = count_by_letter(root, letter);
				cout << "Количество записей, начинающихся с буквы '" << letter << "': " << count << endl;
				wall();
			}
			else if (operation == 8) {
				wall();
				clear();
				int key;
				wall();
				cout << "Введите ключ для поиска: ";
				while (!(cin >> key)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					wall();
					cout << "Ошибка: введите целое число: ";
				}
				wall();
				Element* found = find_element(root, key);
				if (found) {
					cout << "Найден элемент: " << found->key << " " << found->info << endl;
				}
				else {
					cout << "Элемент с таким ключом не найден.\n";
				}
				wall();
			}
			else if (operation == 9) {
				wall();
				clear();
				int key;
				wall();
				cout << "Введите ключ элемента для удаления: ";
				while (!(cin >> key)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					wall();
					cout << "Ошибка: введите целое число: ";
				}
				wall();
				Element* to_delete = find_element(root, key);
				if (to_delete == nullptr) {
					cout << "Ошибка: элемент с таким ключом не найден.\n";
				}
				else {
					root = delete_element(root, key);
					cout << "Удаление выполнено.\n";
				}
				wall();
			}
			else if (operation == 10) {
				wall();
				exit(0);
			}
		clear();
	}
	return 0;
}
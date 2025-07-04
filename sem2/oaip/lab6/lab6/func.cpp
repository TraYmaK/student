#include "func.hpp"
#include <iostream>

int quantity_of_elements = 0;

using namespace std;

void wall() {
	cout << "--------------------------------------------------\n";
}

void clear() {
	system("pause");
	system("cls");
}

Element* builder(Element* root, Element* element) {
	if (root == nullptr) {
		return element;
	}

	if (element->key < root->key) {
		root->left = builder(root->left, element);
	}
	else {
		root->right = builder(root->right, element);
	}
	return root;
}

Element* add_element(Element* root) {
	wall();
	int key;
	cout << "¬ведите ключ дл€ текущего элемента: ";
	while (!(cin >> key)) {
		cout << "ќшибка ввода. ¬ведите целое число: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	cin.ignore();
	wall();
	cout << "¬ведите данные дл€ элемента: ";
	string info;
	getline(cin, info);
	wall();
	Element* element = new Element(key, info);
	root = builder(root, element);
	cout << "Ёлемент успешно добавлен.\n";
	quantity_of_elements++;
	wall();
	return root;
}

Element* create_tree() {
	wall();
	cout << "¬ведите количество элементов: ";
	while (!(cin >> quantity_of_elements) || quantity_of_elements < 0) {
		cout << "ќшибка ввода. ¬ведите неотрицательное целое число: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	int to_add = quantity_of_elements;
	Element* root = nullptr;
	for (int i = 0; i < to_add; i++) {
		root = add_element(root);
		clear();
	}
	wall();
	cout << "ƒерево успешно создано.\n";
	wall();
	return root;
}

void collect_nodes(Element* root, NodeData* arr, int& index) {
	if (root == nullptr) return;
	collect_nodes(root->left, arr, index);
	arr[index].key = root->key;
	arr[index].info = root->info;
	index++;
	collect_nodes(root->right, arr, index);
}

Element* build_balanced_tree(NodeData* arr, int start, int end) {
	if (start > end) return nullptr;
	int mid = (start + end) / 2;
	Element* node = new Element(arr[mid].key, arr[mid].info);
	node->left = build_balanced_tree(arr, start, mid - 1);
	node->right = build_balanced_tree(arr, mid + 1, end);
	return node;
}

int count_nodes(Element* node) {
	if (node == nullptr) return 0;
	return 1 + count_nodes(node->left) + count_nodes(node->right);
}

Element* balance_tree(Element* root) {
	if (root == nullptr) return nullptr;

	int actual_count = count_nodes(root);

	NodeData* arr = new NodeData[actual_count];
	int index = 0;
	collect_nodes(root, arr, index);

	Element* balanced_root = build_balanced_tree(arr, 0, actual_count - 1);
	delete[] arr;
	return balanced_root;
}

void preorder(Element* root) {
	if (root == nullptr) return;
	cout << root->key << " " << root->info << endl;
	preorder(root->left);
	preorder(root->right);
}

void inorder(Element* root) {
	if (root == nullptr) return;
	inorder(root->left);
	cout << root->key << " " << root->info << endl;
	inorder(root->right);
}

void postorder(Element* root) {
	if (root == nullptr) return;
	postorder(root->left);
	postorder(root->right);
	cout << root->key << " " << root->info << endl;
}

int count_by_letter(Element* root, char letter) {
	if (root == nullptr) return 0;

	int count = 0;
	if (!root->info.empty() && tolower(root->info[0]) == tolower(letter)) {
		count = 1;
	}
	return count + count_by_letter(root->left, letter) + count_by_letter(root->right, letter);
}

Element* find_element(Element* root, int key) {
	if (root == nullptr || root->key == key) return root;

	if (key < root->key)
		return find_element(root->left, key);
	else
		return find_element(root->right, key);
}

Element* find_min(Element* root) {
	while (root->left != nullptr)
		root = root->left;
	return root;
}

Element* delete_element(Element* root, int key) {
	if (root == nullptr) return nullptr;

	if (key < root->key) {
		root->left = delete_element(root->left, key);
	}
	else if (key > root->key) {
		root->right = delete_element(root->right, key);
	}
	else {
		if (root->left == nullptr) {
			Element* temp = root->right;
			delete root;
			quantity_of_elements--;
			return temp;
		}
		else if (root->right == nullptr) {
			Element* temp = root->left;
			delete root;
			quantity_of_elements--;
			return temp;
		}
		Element* min_node = find_min(root->right);
		root->key = min_node->key;
		root->info = min_node->info;
		root->right = delete_element(root->right, min_node->key);
	}
	return root;
}

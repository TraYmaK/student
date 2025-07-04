#pragma once

#include <string>

struct Element {
	int key;
	std::string info;
	Element* right;
	Element* left;
	int height;

	Element() {
		key = -1;
		info = "";
		right = nullptr;
		left = nullptr;
		height = 1;
	}

	Element(const int& key_value, const std::string& info_value) {
		key = key_value;
		info = info_value;
		right = nullptr;
		left = nullptr;
		height = 1;
	}
};

struct NodeData {
	int key;
	std::string info;
};

extern int quantity_of_elements;

void wall();
void clear();
Element* builder(Element* root, Element* element);
Element* add_element(Element* root);
Element* create_tree();
Element* balance_tree(Element* root);
void preorder(Element* root);
void inorder(Element* root);
void postorder(Element* root);
int count_by_letter(Element* root, char letter);
Element* delete_element(Element* root, int key);
Element* find_element(Element* root, int key);
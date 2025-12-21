#include "multiset.hpp"
#include <cctype>
#include <string>

using namespace std;

Multiset::Multiset() {
    isAtomNode = false;
    atomValue = '\0';
    children = 0;
    childrenCount = 0;
    childrenCapacity = 0;
}

Multiset::Multiset(const string &text) {
    isAtomNode = false;
    atomValue = '\0';
    children = 0;
    childrenCount = 0;
    childrenCapacity = 0;

    *this = parse(text);
}

Multiset::Multiset(char c) {
    isAtomNode = true;
    atomValue = c;
    children = 0;
    childrenCount = 0;
    childrenCapacity = 0;
}

Multiset::Multiset(const Multiset &other) {
    children = 0;
    childrenCount = 0;
    childrenCapacity = 0;
    copyFrom(other);
}

Multiset& Multiset::operator=(const Multiset &other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Multiset::~Multiset() {
    clear();
}

bool Multiset::isAtom() const {
    return isAtomNode;
}

bool Multiset::isSet() const {
    return !isAtomNode;
}

string Multiset::toString() const {
    if (isAtomNode) {
        string s;
        s.push_back(atomValue);
        return s;
    }

    string result;
    result.push_back(OPEN_BRACE);

    for (int i = 0; i < childrenCount; ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += children[i].toString();
    }

    result.push_back(CLOSE_BRACE);
    return result;
}

void Multiset::clear() {
    if (children != 0) {
        delete[] children;
        children = 0;
    }
    childrenCount = 0;
    childrenCapacity = 0;
}

void Multiset::copyFrom(const Multiset &other) {
    isAtomNode = other.isAtomNode;
    atomValue = other.atomValue;

    if (other.childrenCount == 0) {
        children = 0;
        childrenCount = 0;
        childrenCapacity = 0;
        return;
    }

    childrenCapacity = other.childrenCount;
    childrenCount = other.childrenCount;
    children = new Multiset[childrenCapacity];

    for (int i = 0; i < childrenCount; ++i) {
        children[i] = other.children[i];
    }
}

void Multiset::ensureCapacity(int newSize) {
    if (newSize <= childrenCapacity) {
        return;
    }

    int newCapacity;

    if (childrenCapacity == 0) {
        newCapacity = 4;
    } else {
        newCapacity = childrenCapacity * 2;
    }
    while (newCapacity < newSize) {
        newCapacity *= 2;
    }

    Multiset *newArray = new Multiset[newCapacity];

    for (int i = 0; i < childrenCount; ++i) {
        newArray[i] = children[i];
    }

    if (children != 0) {
        delete[] children;
    }

    children = newArray;
    childrenCapacity = newCapacity;
}

void Multiset::addChild(const Multiset &child) {
    ensureCapacity(childrenCount + 1);
    children[childrenCount] = child;
    childrenCount++;
}

void Multiset::skipSpaces(const string &text, int &position) {
    int n = (int)text.size();
    while (position < n &&
           isspace(static_cast<unsigned char>(text[position]))) {
        position++;
    }
}

Multiset Multiset::parseNode(const string &text, int &position) {
    skipSpaces(text, position);

    if (position >= (int)text.size()) {
        return Multiset();
    }

    if (text[position] == OPEN_BRACE) {
        return parseSet(text, position);
    }

    return parseAtom(text, position);
}

Multiset Multiset::parseSet(const string &text, int &position) {
    Multiset result;

    position++;        // skip '{'
    skipSpaces(text, position);

    if (position < (int)text.size() && text[position] == CLOSE_BRACE) {
        position++;    // "{}"
        return result;
    }

    bool done = false;
    int n = (int)text.size();

    while (!done && position < n) {
        Multiset element = parseNode(text, position);
        result.addChild(element);

        skipSpaces(text, position);
        if (position >= n) {
            done = true;
        } else if (text[position] == COMMA) {
            position++;
            skipSpaces(text, position);
        } else if (text[position] == CLOSE_BRACE) {
            position++;
            done = true;
        } else {
            done = true;
        }
    }

    return result;
}

Multiset Multiset::parseAtom(const string &text, int &position) {
    char c = text[position];
    position++;
    return Multiset(c);
}

Multiset Multiset::parse(const string &text) {
    int position = 0;
    skipSpaces(text, position);
    return parseNode(text, position);
}

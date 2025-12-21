#ifndef MULTISET_HPP
#define MULTISET_HPP

#include <string>

using namespace std;

const char OPEN_BRACE  = '{';
const char CLOSE_BRACE = '}';
const char COMMA       = ',';

class Multiset {
public:
    Multiset();
    explicit Multiset(char c);
    explicit Multiset(const string &text);
    Multiset(const Multiset &other);
    Multiset& operator=(const Multiset &other);
    ~Multiset();

    bool isAtom() const;
    bool isSet() const;
    string toString() const;

private:
    bool isAtomNode;
    char atomValue;

    Multiset *children;
    int childrenCount;
    int childrenCapacity;

    void clear();
    void copyFrom(const Multiset &other);
    void ensureCapacity(int newSize);
    void addChild(const Multiset &child);

    static void skipSpaces(const string &text, int &position);
    static Multiset parseNode(const string &text, int &position);
    static Multiset parseSet(const string &text, int &position);
    static Multiset parseAtom(const string &text, int &position);
    static Multiset parse(const string &text);
};

#endif // MULTISET_HPP

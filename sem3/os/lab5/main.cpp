#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const size_t BLOCK_SIZE = 16;
const size_t BLOCK_COUNT = 128;

struct Block {
    int next;
    char data[BLOCK_SIZE];
};

vector<Block> diskBlocks(BLOCK_COUNT);
vector<bool> blockFree(BLOCK_COUNT, true);

int allocateBlock() {
    for (int i = 0; i < (int)BLOCK_COUNT; ++i) {
        if (blockFree[i]) {
            blockFree[i] = false;
            diskBlocks[i].next = -1;
            for (size_t j = 0; j < BLOCK_SIZE; ++j) {
                diskBlocks[i].data[j] = 0;
            }
            return i;
        }
    }
    return -1;
}

void freeBlockChain(int start) {
    int cur = start;
    while (cur != -1) {
        int nxt = diskBlocks[cur].next;
        blockFree[cur] = true;
        diskBlocks[cur].next = -1;
        cur = nxt;
    }
}

int writeBlockChain(const string &data) {
    if (data.empty()) {
        return -1;
    }
    int first = -1;
    int prev = -1;
    size_t pos = 0;

    while (pos < data.size()) {
        int b = allocateBlock();
        if (b == -1) {
            cerr << "No free blocks on disk\n";
            return first;
        }
        if (first == -1) first = b;
        if (prev != -1) diskBlocks[prev].next = b;

        size_t chunk = min(BLOCK_SIZE, data.size() - pos);
        for (size_t i = 0; i < chunk; ++i) {
            diskBlocks[b].data[i] = data[pos + i];
        }
        pos += chunk;
        prev = b;
    }
    return first;
}

string readBlockChain(int start) {
    string res;
    int cur = start;
    while (cur != -1) {
        res.append(diskBlocks[cur].data, BLOCK_SIZE);
        cur = diskBlocks[cur].next;
    }
    return res;
}

enum class NodeType { File, Directory };

struct Node {
    string name;
    NodeType type;
    Node *parent;

    vector<Node*> children;

    int firstBlock;
    size_t size;

    Node(const string &n, NodeType t, Node *p)
        : name(n), type(t), parent(p), firstBlock(-1), size(0) {}
};

Node *root = nullptr;

vector<string> splitPath(const string &path) {
    vector<string> parts;
    string tmp;
    for (char c : path) {
        if (c == '/') {
            if (!tmp.empty()) {
                parts.push_back(tmp);
                tmp.clear();
            }
        } else {
            tmp.push_back(c);
        }
    }
    if (!tmp.empty()) parts.push_back(tmp);
    return parts;
}

Node* findChild(Node *dir, const string &name) {
    for (Node* ch : dir->children) {
        if (ch->name == name) return ch;
    }
    return nullptr;
}

Node* resolvePath(const string &path) {
    if (path == "/") return root;
    if (path.empty() || path[0] != '/') return nullptr;

    vector<string> parts = splitPath(path);
    Node *cur = root;
    for (const string &p : parts) {
        if (!cur || cur->type != NodeType::Directory) return nullptr;
        cur = findChild(cur, p);
    }
    return cur;
}

Node* resolveParent(const string &path, string &last) {
    if (path.empty() || path[0] != '/') return nullptr;
    vector<string> parts = splitPath(path);
    if (parts.empty()) return nullptr;
    last = parts.back();
    parts.pop_back();

    Node *cur = root;
    for (const string &p : parts) {
        if (!cur || cur->type != NodeType::Directory) return nullptr;
        cur = findChild(cur, p);
    }
    return cur;
}

bool createDir(const string &path) {
    string name;
    Node *parent = resolveParent(path, name);
    if (!parent || parent->type != NodeType::Directory) return false;
    if (findChild(parent, name)) return false;
    Node *dir = new Node(name, NodeType::Directory, parent);
    parent->children.push_back(dir);
    return true;
}

bool createFile(const string &path) {
    string name;
    Node *parent = resolveParent(path, name);
    if (!parent || parent->type != NodeType::Directory) return false;
    if (findChild(parent, name)) return false;
    Node *f = new Node(name, NodeType::File, parent);
    parent->children.push_back(f);
    return true;
}

void deleteNodeRec(Node *node) {
    if (node->type == NodeType::File) {
        if (node->firstBlock != -1) {
            freeBlockChain(node->firstBlock);
        }
    } else {
        for (Node *ch : node->children) {
            deleteNodeRec(ch);
        }
        node->children.clear();
    }
    delete node;
}

bool removePath(const string &path) {
    if (path == "/") return false;
    Node *node = resolvePath(path);
    if (!node || !node->parent) return false;

    vector<Node*> &sibs = node->parent->children;
    sibs.erase(remove(sibs.begin(), sibs.end(), node), sibs.end());
    deleteNodeRec(node);
    return true;
}

bool writeFile(const string &path, const string &data) {
    Node *node = resolvePath(path);
    if (!node || node->type != NodeType::File) return false;

    if (node->firstBlock != -1) {
        freeBlockChain(node->firstBlock);
        node->firstBlock = -1;
    }
    if (!data.empty()) {
        int first = writeBlockChain(data);
        if (first == -1) return false;
        node->firstBlock = first;
    }
    node->size = data.size();
    return true;
}

bool readFile(const string &path, string &out) {
    Node *node = resolvePath(path);
    if (!node || node->type != NodeType::File) return false;
    if (node->firstBlock == -1) {
        out.clear();
        return true;
    }
    string raw = readBlockChain(node->firstBlock);
    if (raw.size() < node->size) {
        out = raw;
    } else {
        out = raw.substr(0, node->size);
    }
    return true;
}

bool copyFile(const string &src, const string &dst) {
    Node *srcNode = resolvePath(src);
    if (!srcNode || srcNode->type != NodeType::File) return false;

    string dstName;
    Node *dstParent = resolveParent(dst, dstName);
    if (!dstParent || dstParent->type != NodeType::Directory) return false;
    if (findChild(dstParent, dstName)) return false;

    string data;
    if (!readFile(src, data)) return false;

    Node *dstNode = new Node(dstName, NodeType::File, dstParent);
    dstParent->children.push_back(dstNode);
    return writeFile(dst, data);
}

bool movePath(const string &src, const string &dst) {
    if (src == "/") return false;
    Node *node = resolvePath(src);
    if (!node || !node->parent) return false;

    string newName;
    Node *newParent = resolveParent(dst, newName);
    if (!newParent || newParent->type != NodeType::Directory) return false;
    if (findChild(newParent, newName)) return false;

    vector<Node*> &oldSibs = node->parent->children;
    oldSibs.erase(remove(oldSibs.begin(), oldSibs.end(), node), oldSibs.end());

    node->parent = newParent;
    node->name = newName;
    newParent->children.push_back(node);
    return true;
}

void dumpTree(Node *node, int depth, const string &pathPrefix) {
    string indent(depth * 2, ' ');
    string fullPath = (pathPrefix == "/" ? "/" + node->name : pathPrefix + "/" + node->name);
    if (node == root) fullPath = "/";

    if (node->type == NodeType::Directory) {
        cout << indent << fullPath << "/\n";
        for (Node *ch : node->children) {
            dumpTree(ch, depth + 1, fullPath);
        }
    } else {
        cout << indent << fullPath << " (size=" << node->size
             << ", firstBlock=" << node->firstBlock << ")\n";

        string content;
        if (readFile(fullPath, content)) {
            cout << indent << "  content: ";
            const size_t maxPrint = 64;
            if (content.size() > maxPrint) {
                cout << content.substr(0, maxPrint) << "...";
            } else {
                cout << content;
            }
            cout << "\n";
        }
    }
}

void dumpBlocks() {
    cout << "=== Blocks ===\n";
    for (int i = 0; i < (int)BLOCK_COUNT; ++i) {
        if (!blockFree[i]) {
            cout << "[" << i << "] next=" << diskBlocks[i].next << " data=\"";
            for (size_t j = 0; j < BLOCK_SIZE; ++j) {
                char c = diskBlocks[i].data[j];
                if (c == 0) break;
                if (c == '\n') cout << "\\n";
                else cout << c;
            }
            cout << "\"\n";
        }
    }
}

void printHelp() {
    cout << "Commands:\n";
    cout << "  mkdir PATH\n";
    cout << "  create PATH\n";
    cout << "  rm PATH\n";
    cout << "  write PATH TEXT...\n";
    cout << "  read PATH\n";
    cout << "  cp SRC DST\n";
    cout << "  mv SRC DST\n";
    cout << "  dump\n";
    cout << "  exit\n";
}

int main() {
    root = new Node("", NodeType::Directory, nullptr);

    cout << "Simple FS. Type 'help' for commands.\n";
    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "help") {
            printHelp();
        } else if (cmd == "mkdir") {
            string path; ss >> path;
            if (!createDir(path)) cout << "Error\n";
        } else if (cmd == "create") {
            string path; ss >> path;
            if (!createFile(path)) cout << "Error\n";
        } else if (cmd == "rm") {
            string path; ss >> path;
            if (!removePath(path)) cout << "Error\n";
        } else if (cmd == "write") {
            string path; ss >> path;
            string rest;
            getline(ss, rest);
            if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
            if (!writeFile(path, rest)) cout << "Error\n";
        } else if (cmd == "read") {
            string path; ss >> path;
            string data;
            if (!readFile(path, data)) cout << "Error\n";
            else cout << data << "\n";
        } else if (cmd == "cp") {
            string s, d; ss >> s >> d;
            if (!copyFile(s, d)) cout << "Error\n";
        } else if (cmd == "mv") {
            string s, d; ss >> s >> d;
            if (!movePath(s, d)) cout << "Error\n";
        } else if (cmd == "dump") {
            cout << "=== Tree ===\n";
            dumpTree(root, 0, "/");
            dumpBlocks();
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
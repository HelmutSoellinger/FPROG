#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

class ImmutableRBTree {
public:
    struct Node {
        int color;  // 0 - Black, 1 - Red
        std::string key;
        const Node* left;
        const Node* right;
        const Node* parent;

        Node(int color, const std::string& key, const Node* left = nullptr, const Node* right = nullptr, const Node* parent = nullptr)
            : color(color), key(key), left(left), right(right), parent(parent) {}
    };

    static const Node* empty() { return nullptr; }

    template<typename T>
    static const Node* insert(const Node* tree, const T& key);

    template<typename T>
    static void inorderTraversal(const Node* tree, std::vector<T>& result);

private:
    static const Node* newNode(const std::string& key);
    static const Node* leftRotate(const Node* x);
    static const Node* rightRotate(const Node* x);
    static const Node* fixInsert(const Node* k);
};

template<typename T>
const ImmutableRBTree::Node* ImmutableRBTree::insert(const ImmutableRBTree::Node* tree, const T& key) {
    auto node = newNode(key);
    auto y = nullptr;
    auto x = tree;

    while (x != nullptr) {
        y = x;
        if (node->key < x->key) {
            x = x->left;
        }
        else if (node->key > x->key) {
            x = x->right;
        }
        else {
            return tree; // Key already exists
        }
    }

    node->parent = y;
    if (y == nullptr) {
        return node;
    }
    else if (node->key < y->key) {
        return new ImmutableRBTree::Node(y->color, y->key, node, y->right, y->parent);
    }
    else {
        return new ImmutableRBTree::Node(y->color, y->key, y->left, node, y->parent);
    }
}

template<typename T>
void ImmutableRBTree::inorderTraversal(const ImmutableRBTree::Node* tree, std::vector<T>& result) {
    if (tree != nullptr) {
        inorderTraversal(tree->left, result);
        result.push_back(tree->key);
        inorderTraversal(tree->right, result);
    }
}

const ImmutableRBTree::Node* ImmutableRBTree::newNode(const std::string& key) {
    return new ImmutableRBTree::Node(1, key, nullptr, nullptr, nullptr);
}

const ImmutableRBTree::Node* ImmutableRBTree::leftRotate(const ImmutableRBTree::Node* x) {
    auto y = x->right;
    auto new_x = new ImmutableRBTree::Node(x->color, x->key, x->left, y->left, x->parent);
    auto new_y = new ImmutableRBTree::Node(y->color, y->key, new_x, y->right, y->parent);

    if (x->parent == nullptr) {
        return new_y;
    }
    else if (x == x->parent->left) {
        x->parent->left = new_y;
    }
    else {
        x->parent->right = new_y;
    }

    return new_y;
}

const ImmutableRBTree::Node* ImmutableRBTree::rightRotate(const ImmutableRBTree::Node* x) {
    auto y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        return y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    return y;
}

const ImmutableRBTree::Node* ImmutableRBTree::fixInsert(const ImmutableRBTree::Node* k) {
    ImmutableRBTree::Node* u;
    while (k->parent != nullptr && k->color == 1) {
        if (k->parent == k->parent->right) {
            u = k->parent->left;
            if (u != nullptr && u->color == 1) {
                u->color = 0;
                k->parent->color = 0;
                k->parent->parent->color = 1;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->left) {
                    k = k->parent;
                    k = ImmutableRBTree::rightRotate(k);
                }
                if (k->parent->parent != nullptr) {
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k->parent = ImmutableRBTree::leftRotate(k->parent);
                }
            }
        }
        else {
            u = k->parent->right;
            if (u != nullptr && u->color == 1) {
                u->color = 0;
                k->parent->color = 0;
                k->parent->parent->color = 1;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->right) {
                    k = k->parent;
                    k = ImmutableRBTree::leftRotate(k);
                }
                if (k->parent->parent != nullptr) {
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k->parent = ImmutableRBTree::rightRotate(k->parent);
                }
            }
        }
        if (k == nullptr) {
            break;
        }
    }
    return k;
}
// Function to read the text file
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to tokenize the text
std::vector<std::string> tokenizeText(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word) {
        // Convert to lowercase and remove punctuation
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char c) { return std::tolower(c); });
        word.erase(std::remove_if(word.begin(), word.end(),
            [](char c) { return !std::isalnum(c); }), word.end());
        if (!word.empty()) {
            tokens.push_back(word);
        }
    }

    return tokens;
}

// Function to insert unique words into the RBTree
ImmutableRBTree::Node* insertUniqueWords(ImmutableRBTree::Node* tree, const std::vector<std::string>& tokens) {
    for (const auto& word : tokens) {
        tree = ImmutableRBTree::insert(tree, word);
    }
    return tree;
}

// Function to write sorted list to file
void writeToFile(const std::vector<std::string>& sortedList, const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& word : sortedList) {
        outFile << word << '\n';
    }
    outFile.close();
}

int main() {
    auto text = readFile("war_and_peace.txt");
    auto tokens = tokenizeText(text);
    auto tree = insertUniqueWords(ImmutableRBTree::empty(), tokens);
    std::vector<std::string> sortedList;
    ImmutableRBTree::inorderTraversal(tree, sortedList);
    writeToFile(sortedList, "output.txt");

    return 0;
}
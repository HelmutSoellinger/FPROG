#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

// Node Structure
struct Node {
    const int color;  // 0 - Black, 1 - Red
    const std::string key;
    const Node* left;
    const Node* right;

    Node(const std::string& k, int c, const Node* l = nullptr, const Node* r = nullptr)
        : color(c), key(k), left(l), right(r) {}
};

// Create a new Node
auto createNode = [](const std::string& key, int color, const Node* left = nullptr, const Node* right = nullptr) -> const Node* {
    return new Node(key, color, left, right);
};

// Recursive Insertion
auto insertRecursive = [](const auto& self, const Node* current, const std::string& key) -> const Node* {
    if (!current) return createNode(key, 1); // New node is red
    if (key < current->key) {
        return createNode(current->key, current->color, self(self, current->left, key), current->right);
    } else if (key > current->key) {
        return createNode(current->key, current->color, current->left, self(self, current->right, key));
    }
    return current; // Key already exists
};

// Fix Violations after Insertion
auto fixInsert = [](const Node* node, const Node* parent) -> const Node* {
    if (!node || !parent || parent->color == 0) return node;

    const Node* grandparent = parent->left == node ? parent->left : parent->right;
    const Node* uncle = (grandparent && grandparent->left == parent) ? grandparent->right : grandparent->left;

    if (uncle && uncle->color == 1) {
        return createNode(grandparent->key, 1,
                          createNode(parent->key, 0, parent->left, parent->right),
                          createNode(uncle->key, 0, uncle->left, uncle->right));
    }

    return node;
};

// Rotations
auto leftRotate = [](const Node* x) -> const Node* {
    const Node* y = x->right;
    return createNode(y->key, x->color, createNode(x->key, x->color, x->left, y->left), y->right);
};

auto rightRotate = [](const Node* x) -> const Node* {
    const Node* y = x->left;
    return createNode(y->key, x->color, y->left, createNode(x->key, x->color, y->right, x->right));
};

// In-order Traversal
auto inorderTraversal = [](const auto& self, const Node* node) -> std::vector<std::string> {
    if (!node) return {};
    std::vector<std::string> left = self(self, node->left);
    left.push_back(node->key);
    std::vector<std::string> right = self(self, node->right);
    left.insert(left.end(), right.begin(), right.end());
    return left;
};

class RBTree {
public:
    const Node* root;

    RBTree() : root(nullptr) {}

    RBTree insert(const std::string& key) const {
        // Recursive insertion to create a new root
        const Node* newRoot = insertRecursive(insertRecursive, root, key);

        // Fix any violations in the tree structure
        if (newRoot) {
            newRoot = fixInsert(newRoot, nullptr); // Call fixInsert with the correct arguments
        }

        // Return a new tree with the updated root
        RBTree newTree;
        newTree.root = newRoot;
        return newTree;
    }

    std::vector<std::string> inorder() const {
        return inorderTraversal(inorderTraversal, root);
    }
};

// File Reading
auto readFile = [](const std::string& filename) -> std::string {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
};

// Tokenization
auto tokenizeText = [](const std::string& text) -> std::vector<std::string> {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string word;

    auto isAlnum = [](char c) { return std::isalnum(c); };
    auto toLower = [](unsigned char c) { return std::tolower(c); };

    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), toLower);
        word.erase(std::remove_if(word.begin(), word.end(), [&](char c) { return !isAlnum(c); }), word.end());
        if (!word.empty()) tokens.push_back(word);
    }
    return tokens;
};

// File Writing
auto writeToFile = [](const std::vector<std::string>& sortedList, const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& word : sortedList) {
        outFile << word << '\n';
    }
    outFile.close();
};

// Main Function
int main() {
    const std::string text = readFile("war_and_peace.txt");
    const auto tokens = tokenizeText(text);

    RBTree tree = std::accumulate(tokens.begin(), tokens.end(), RBTree(),
                                  [](const RBTree& acc, const std::string& word) {
                                      return acc.insert(word);
                                  });

    const auto sortedList = tree.inorder();
    writeToFile(sortedList, "output.txt");

    std::cout << "Processing completed. Sorted list written to output.txt." << std::endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

// Immutable Node structure
struct Node {
    const int color; // 0 - Black, 1 - Red
    const std::string key;
    const Node* left;
    const Node* right;

    Node(const std::string& k, int c, const Node* l = nullptr, const Node* r = nullptr)
        : color(c), key(k), left(l), right(r) {}
};

// Create a new node
auto createNode = [](const std::string& key, int color, const Node* left = nullptr, const Node* right = nullptr) -> const Node* {
    return new Node(key, color, left, right);
};

// Left rotation
auto leftRotate = [](const Node* x) -> const Node* {
    const Node* y = x->right;
    return createNode(y->key, x->color,
                      createNode(x->key, 1, x->left, y->left), y->right);
};

// Right rotation
auto rightRotate = [](const Node* x) -> const Node* {
    const Node* y = x->left;
    return createNode(y->key, x->color,
                      y->left, createNode(x->key, 1, y->right, x->right));
};

// Recursive insertion
auto insert = [](const auto& self, const Node* root, const std::string& key) -> const Node* {
    if (!root) return createNode(key, 1); // New nodes are red by default

    if (key < root->key)
        return createNode(root->key, root->color, self(self, root->left, key), root->right);
    if (key > root->key)
        return createNode(root->key, root->color, root->left, self(self, root->right, key));

    return root; // Key already exists
};

// Fix Red-Black Tree violations
auto fixInsert = [](const Node* node) -> const Node* {
    if (!node) return node;

    // If the left child and its left child are red, perform right rotation
    if (node->left && node->left->color == 1 && node->left->left && node->left->left->color == 1) {
        return rightRotate(createNode(node->key, 1, node->left, node->right));
    }

    // If the right child and its right child are red, perform left rotation
    if (node->right && node->right->color == 1 && node->right->right && node->right->right->color == 1) {
        return leftRotate(createNode(node->key, 1, node->left, node->right));
    }

    // If the left child and its right child are red, perform left-right rotation
    if (node->left && node->left->color == 1 && node->left->right && node->left->right->color == 1) {
        return rightRotate(createNode(node->key, 1, leftRotate(node->left), node->right));
    }

    // If the right child and its left child are red, perform right-left rotation
    if (node->right && node->right->color == 1 && node->right->left && node->right->left->color == 1) {
        return leftRotate(createNode(node->key, 1, node->left, rightRotate(node->right)));
    }

    return node; // No violations
};

// Insert with fix
auto insertWithFix = [](const auto& insertFn, const auto& fixFn, const Node* root, const std::string& key) -> const Node* {
    auto newRoot = insertFn(insertFn, root, key);
    newRoot = fixFn(newRoot);  // Only pass `newRoot` to `fixFn`

    // Ensure the root is black
    if (newRoot && newRoot->color == 1) {
        newRoot = createNode(newRoot->key, 0, newRoot->left, newRoot->right);
    }

    return newRoot;
};

// In-order traversal
auto inorderTraversal = [](const auto& self, const Node* root) -> std::vector<std::string> {
    if (!root) return {};
    auto left = self(self, root->left);
    left.push_back(root->key);
    auto right = self(self, root->right);
    left.insert(left.end(), right.begin(), right.end());
    return left;
};

// Read text from a file
auto readFile = [](const std::string& filename) -> std::string {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
};

// Tokenize the text
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

// Write sorted list to a file
auto writeToFile = [](const std::vector<std::string>& sortedList, const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& word : sortedList) {
        outFile << word << '\n';
    }
    outFile.close();
};

int main() {
    // Read text from file
    const std::string text = readFile("war_and_peace.txt");

    // Tokenize text into individual words
    const auto tokens = tokenizeText(text);

    // Use accumulate to construct the Red-Black Tree
    const Node* tree = std::accumulate(tokens.begin(), tokens.end(), static_cast<const Node*>(nullptr),
                                       [](const Node* acc, const std::string& word) {
                                           return insertWithFix(insert, fixInsert, acc, word);
                                       });

    // Perform in-order traversal to get sorted list of words
    const auto sortedList = inorderTraversal(inorderTraversal, tree);

    // Write the sorted list to the output file
    writeToFile(sortedList, "output.txt");

    std::cout << "Processing completed. Sorted list written to output.txt." << std::endl;
    return 0;
}

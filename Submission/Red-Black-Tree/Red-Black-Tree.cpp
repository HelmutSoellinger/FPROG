#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

// Red-Black Tree Node (Immutable)
struct Node {
    const int color;  // 0 - Black, 1 - Red
    const std::string key;
    const Node* left;
    const Node* right;

    Node(const std::string& k, int c, const Node* l = nullptr, const Node* r = nullptr)
        : color(c), key(k), left(l), right(r) {}
};

// Persistent Red-Black Tree
class RBTree {
public:
    const Node* root;

    // Constructor
    RBTree() : root(nullptr) {}

    // Functional insertion: returns a new tree
    RBTree insert(const std::string& key) const {
        // Lambda for recursive insertion
        auto insertRecursive = [&](const auto& self, const Node* current, const std::string& key) -> const Node* {
            if (!current) return new Node(key, 1); // New node is red
            if (key < current->key) {
                return new Node(current->key, current->color, self(self, current->left, key), current->right);
            } else if (key > current->key) {
                return new Node(current->key, current->color, current->left, self(self, current->right, key));
            }
            return current; // Key already exists
        };

        // Apply recursive insertion
        const Node* newRoot = insertRecursive(insertRecursive, root, key);

        // Lambda for fixing violations
        auto fixInsert = [&](const Node* node, const Node* parent) -> const Node* {
            if (!node || !parent || parent->color == 0) return node;

            // Get sibling and grandparent
            const Node* grandparent = parent->left == node ? parent->left : parent->right;
            const Node* uncle = (grandparent && grandparent->left == parent) ? grandparent->right : grandparent->left;

            // Case: Recolor
            if (uncle && uncle->color == 1) {
                return new Node(grandparent->key, 1,
                                new Node(parent->key, 0, parent->left, parent->right),
                                new Node(uncle->key, 0, uncle->left, uncle->right));
            }

            // Case: Rotations
            if (node == parent->right && parent == grandparent->left) return leftRotate(parent);
            if (node == parent->left && parent == grandparent->right) return rightRotate(parent);

            return node;
        };

        // Apply fixes if necessary
        if (newRoot) newRoot = fixInsert(newRoot, nullptr);

        // Return the new tree
        RBTree newTree;
        newTree.root = newRoot;
        return newTree;
    }

    // In-order Traversal
    std::vector<std::string> inorderTraversal() const {
        // Lambda for recursive traversal
        auto traverse = [&](const auto& self, const Node* node) -> std::vector<std::string> {
            if (!node) return {};
            std::vector<std::string> left = self(self, node->left);
            left.push_back(node->key);
            std::vector<std::string> right = self(self, node->right);
            left.insert(left.end(), right.begin(), right.end());
            return left;
        };

        return traverse(traverse, root);
    }

private:
    // Left rotation
    const Node* leftRotate(const Node* x) const {
        const Node* y = x->right;
        return new Node(y->key, x->color, new Node(x->key, x->color, x->left, y->left), y->right);
    }

    // Right rotation
    const Node* rightRotate(const Node* x) const {
        const Node* y = x->left;
        return new Node(y->key, x->color, y->left, new Node(x->key, x->color, y->right, x->right));
    }
};

// Function to read the text file
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Tokenization with a lambda
std::vector<std::string> tokenizeText(const std::string& text) {
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
}

// Function to write the sorted list to a file
void writeToFile(const std::vector<std::string>& sortedList, const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& word : sortedList) {
        outFile << word << '\n';
    }
    outFile.close();
}

int main() {
    // Read the text file
    const std::string text = readFile("war_and_peace.txt");

    // Tokenize the text
    const auto tokens = tokenizeText(text);

    // Create and populate the tree functionally
    RBTree tree = std::accumulate(tokens.begin(), tokens.end(), RBTree(),
                                  [](const RBTree& acc, const std::string& word) {
                                      return acc.insert(word);
                                  });

    // Perform an inorder traversal to retrieve sorted keys
    const auto sortedList = tree.inorderTraversal();

    // Write the sorted list to a file
    writeToFile(sortedList, "output.txt");

    std::cout << "Processing completed. Sorted list written to output.txt." << std::endl;

    return 0;
}

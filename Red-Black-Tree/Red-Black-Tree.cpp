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
        : key(k), color(c), left(l), right(r) {}
};

// Persistent Red-Black Tree
class RBTree {
public:
    const Node* root;

    // Constructor
    RBTree() : root(nullptr) {}

    // Functional insertion: returns a new tree
    RBTree insert(const std::string& key) const {
        RBTree newTree;
        newTree.root = insertRecursive(root, key);
        if (newTree.root) {
            newTree.root = fixInsert(newTree.root, nullptr);
        }
        return newTree;
    }

    void inorderTraversalHelper(const Node* node, std::vector<std::string>& result) const {
        if (node) {
            inorderTraversalHelper(node->left, result);
            result.push_back(node->key);
            inorderTraversalHelper(node->right, result);
        }
    }

    std::vector<std::string> inorderTraversal() const {
        std::vector<std::string> result;
        inorderTraversalHelper(root, result);
        return result;
    }

private:
    // Recursive insertion: creates a new subtree
    const Node* insertRecursive(const Node* current, const std::string& key) const {
        if (!current) {
            return new Node(key, 1); // New node is red
        }
        if (key < current->key) {
            return new Node(current->key, current->color,
                insertRecursive(current->left, key), current->right);
        }
        else if (key > current->key) {
            return new Node(current->key, current->color,
                current->left, insertRecursive(current->right, key));
        }
        return current; // Key already exists, return the current node
    }

    // Fix violations after insertion (ensures tree balance)
    const Node* fixInsert(const Node* node, const Node* parent) const {
        if (!node || !parent || parent->color == 0) {
            return node;
        }

        // Get sibling node
        const Node* grandparent = parent->left == node ? parent->left : parent->right;
        const Node* uncle = (grandparent && grandparent->left == parent) ? grandparent->right : grandparent->left;

        // Case: Uncle is red (Recolor)
        if (uncle && uncle->color == 1) {
            parent = new Node(parent->key, 0, parent->left, parent->right);
            const Node* newUncle = new Node(uncle->key, 0, uncle->left, uncle->right);
            const Node* newGrandparent = new Node(grandparent->key, 1, parent, newUncle);
            return newGrandparent;
        }

        // Case: Rotation needed (adjust structure)
        if (node == parent->right && parent == grandparent->left) {
            return leftRotate(parent);
        }
        else if (node == parent->left && parent == grandparent->right) {
            return rightRotate(parent);
        }

        return node; // No changes needed
    }

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

// Function to read the text file (pure function if no side effects are introduced here)
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Pure function to tokenize the text
std::vector<std::string> tokenizeText(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char c) { return std::tolower(c); });
        word.erase(std::remove_if(word.begin(), word.end(),
            [](char c) { return !std::isalnum(c); }),
            word.end());
        if (!word.empty()) {
            tokens.push_back(word);
        }
    }
    return tokens;
}

// Function to write the sorted list to a file (side-effect isolated)
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
    RBTree tree;
    tree = std::accumulate(tokens.begin(), tokens.end(), tree,
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
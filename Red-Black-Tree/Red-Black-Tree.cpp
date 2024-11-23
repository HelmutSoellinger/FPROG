#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// Persistent Red-Black Tree Node
struct Node {
    int color;  // 0 - Black, 1 - Red
    std::string key;
    Node* left;
    Node* right;
    Node* parent;
};

// Persistent Red-Black Tree
class RBTree {
public:
    Node* root;

    RBTree() : root(nullptr) {}

    // Helper function to create a new node
    static Node* newNode(const std::string& key) {
        Node* node = new Node();
        node->color = 1;  // New node is always red
        node->key = key;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        return node;
    }

    // Left rotation
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Right rotation
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Fix the tree after insertion
    void fixInsert(Node* k) {
        Node* u;
        while (k->parent != nullptr && k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u != nullptr && u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    if (k->parent->parent != nullptr) {
                        k->parent->color = 0;
                        k->parent->parent->color = 1;
                        leftRotate(k->parent->parent);
                    }
                }
            }
            else {
                u = k->parent->parent->right;
                if (u != nullptr && u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    if (k->parent->parent != nullptr) {
                        k->parent->color = 0;
                        k->parent->parent->color = 1;
                        rightRotate(k->parent->parent);
                    }
                }
            }
            if (k == root) {
                break;
            }
        }
        if (root != nullptr) {
            root->color = 0;
        }
    }


    // Insert a new node
    void insert(const std::string& key) {
        Node* node = newNode(key);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (node->key < x->key) {
                x = x->left;
            }
            else if (node->key > x->key) {
                x = x->right;
            }
            else { // Key already exists
                // Instead of inserting, we could just return
                // Or we could keep a count of occurrences
                return;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->key < y->key) {
            y->left = node;
        }
        else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        fixInsert(node);
    }


    // Inorder traversal to get sorted list of keys
    void inorderTraversal(std::vector<std::string>& result) {
        std::function<void(Node*)> traverse = [&](Node* node) {
            if (node != nullptr) {
                traverse(node->left);
                result.push_back(node->key);
                traverse(node->right);
            }
            };
        traverse(root);
    }
};

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
void insertUniqueWords(RBTree& tree, const std::vector<std::string>& tokens) {
    for (const auto& word : tokens) {
        tree.insert(word);
    }
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
    // Read the text file
    std::string text = readFile("war_and_peace.txt");

    // Tokenize the text
    std::vector<std::string> tokens = tokenizeText(text);

    // Create and populate the RBTree
    RBTree tree;
    insertUniqueWords(tree, tokens);

    // Get sorted list from RBTree
    std::vector<std::string> sortedList;
    tree.inorderTraversal(sortedList);

    // Write sorted list to file
    writeToFile(sortedList, "output.txt");

    std::cout << "Processing completed. Sorted list written to output.txt." << std::endl;

    return 0;
}

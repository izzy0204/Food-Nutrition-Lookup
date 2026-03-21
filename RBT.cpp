#include "RBT.h"
#include <algorithm>
#include <cctype>

RBT::RBT() : root(nullptr), nodeCount(0) {}

RBT::~RBT() {
    clear(root);
}

//code taken from CLI.cpp.
string RBT::lowerCase(const string& str) const {
    string res = str;
    transform(res.begin(), res.end(), res.begin(),
        [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return res;
}

void RBT::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

RBT::Node* RBT::findNodeHelper(const string& key) const {
    //compares strings lexigraphically.
    string lowerKey = lowerCase(key);
    Node* current = root;

    while (current != nullptr) {
        if (lowerKey == current->key) {
            return current;
        }
        else if (lowerKey < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

vector<const FoodItem*> RBT::find(const string& key) const {
    Node* node = findNodeHelper(key);
    if (node == nullptr) return {};
    return node->values;
}

void RBT::buildFromFoods(const vector<FoodItem>& foods) {
    for (const auto& food : foods) {
        if (!food.name.empty()) {
            insert(food.name, &food);
        }
    }
}

size_t RBT::size() const {
    return nodeCount;
}

bool RBT::empty() const {
    return nodeCount == 0;
}


void RBT::insert(const string& key, const FoodItem* food) {
    if (food == nullptr) return;

    string lowerKey = lowerCase(key);

    Node* parent = nullptr;
    Node* current = root;

    while (current != nullptr) {
        parent = current;

        if (lowerKey == current->key) {
            //vector is used to make a fair comparison with hashtable.
            //also some keys with same name but diff info may repeat in the large dataset, so vector used to be safe.
            current->values.push_back(food);
            return;
        }
        else if (lowerKey < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Node* newNode = new Node(lowerKey, food);
    newNode->parent = parent;

    if (parent == nullptr) {
        root = newNode;
    }
    else if (lowerKey < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    fixInsert(newNode);
    nodeCount++;
}
/*===========================ROTATIONS/FIXES================================ */
//NOTE: THE CODE FOR THE ROTATION/INSERTION FIXES ARE OBTAINED HERE:
//https://github.com/yassiommi/redblacktree/blob/main/RedBlackTree.h
//Additional Source of RBT: https://www.delftstack.com/howto/cpp/red-black-tree-cpp/
//Slight modification, but code is mostly from those 2 sites.
void RBT::leftRotate(Node* x) {
    if (x == nullptr || x->right == nullptr) {
        return;
    }

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

void RBT::rightRotate(Node* y) {
    if (y == nullptr || y->left == nullptr) {
        return;
    }

    Node* x = y->left;
    y->left = x->right;

    if (x->right != nullptr) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == nullptr) {
        root = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

void RBT::fixInsert(Node* z) {
    while (z != root && z->parent != nullptr && z->parent->color == RED) {
        Node* parent = z->parent;
        Node* grandparent = parent->parent;

        if (grandparent == nullptr) {
            break;
        }

        // parent is left child of grandparent
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

            // Case 1: uncle is red
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            }
            else {
                // Case 2: z is right child
                if (z == parent->right) {
                    z = parent;
                    leftRotate(z);
                    parent = z->parent;
                    grandparent = parent != nullptr ? parent->parent : nullptr;
                }

                // Case 3: z is left child
                if (parent != nullptr) {
                    parent->color = BLACK;
                }
                if (grandparent != nullptr) {
                    grandparent->color = RED;
                    rightRotate(grandparent);
                }
            }
        }
        // parent is right child of grandparent
        else {
            Node* uncle = grandparent->left;

            // Case 1: uncle is red
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            }
            else {
                // Case 2: z is left child
                if (z == parent->left) {
                    z = parent;
                    rightRotate(z);
                    parent = z->parent;
                    grandparent = parent != nullptr ? parent->parent : nullptr;
                }

                // Case 3: z is right child
                if (parent != nullptr) {
                    parent->color = BLACK;
                }
                if (grandparent != nullptr) {
                    grandparent->color = RED;
                    leftRotate(grandparent);
                }
            }
        }
    }

    if (root != nullptr) {
        root->color = BLACK;
    }
}
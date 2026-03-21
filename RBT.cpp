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
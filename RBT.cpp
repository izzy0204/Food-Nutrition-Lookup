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
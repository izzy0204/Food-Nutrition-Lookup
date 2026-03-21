#ifndef PROJECT2B_RBT_H
#define PROJECT2B_RBT_H

#include "FoodItem.h"
#include <vector>

//RBT implementation format taken from the following:
//https://www.delftstack.com/howto/cpp/red-black-tree-cpp/
//https://github.com/yassiommi/redblacktree/blob/main/RedBlackTree.h
class RBT {
    private:
    //enum is a custom data type limited to 2 values: Red and Black.
        enum Color { RED, BLACK };

        struct Node {
            string key;
            vector<const FoodItem*> values;
            Color color;
            Node* left;
            Node* right;
            Node* parent;

            Node(const string& key, const FoodItem* food): key(key), values{food}, color(RED),
                  left(nullptr), right(nullptr), parent(nullptr) {}
        };

        Node* root;
        size_t nodeCount;

        string lowerCase(const string& str) const;

        void leftRotate(Node* x);
        void rightRotate(Node* y);
        void fixInsert(Node* z);

        Node* findNodeHelper(const string& key) const;
        void clear(Node* node);

    public:
        RBT();
        ~RBT();

        RBT(const RBT&) = delete;
        RBT& operator=(const RBT&) = delete;

        void insert(const string& key, const FoodItem* food);
        vector<const FoodItem*> find(const string& key) const;
        void buildFromFoods(const vector<FoodItem>& foods);

        size_t size() const;
        bool empty() const;
    };








#endif //PROJECT2B_RBT_H
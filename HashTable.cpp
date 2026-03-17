#include "HashTable.h"
#include <algorithm>
#include <cctype>

HashTable::HashTable(size_t firstBucketCount) : buckets(firstBucketCount, nullptr), keys(0) {}

HashTable::~HashTable() {
    clearOut();
}

void HashTable::clearOut() {
    for (Node* head : buckets) {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    for (size_t i = 0; i < buckets.size(); i++) {
        buckets[i] = nullptr;
    }

    keys = 0;
}

std::string HashTable::lowerCase(const std::string& str) {
    //finsih later
}


//utilized DJB hash: https://helloacm.com/the-simplest-string-hash-function-djb2-algorithm-and-implementations/
std::size_t HashTable::hashFunction(const std::string& key) {
    unsigned long hash = 5381;

    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % buckets.size();
}

double HashTable::loadFactor() {
    if (buckets.empty()) {
        return 0.0;
    }
    return static_cast<double>(keys) / static_cast<double>(buckets.size());
}

//used this for implementation of rehash: https://www.geeksforgeeks.org/dsa/load-factor-and-rehashing/
void HashTable::rehash() {
    std::vector<Node*> temp = buckets;
    buckets.resize(2 * buckets.size());
    for (size_t i = 0; i < buckets.size() * 2; i++) {
        buckets[i] = nullptr;
    }

    keys = 0;
    for (size_t i = 0; i < temp.size(); i++) {
        Node* head = temp[i];
        while (head != nullptr) {
            for (const FoodItem* food : head->values) {
                insert(head->key, food); // not yet implemented
            }
            Node* nextNode = head->next;
            delete head;
            head = nextNode;
        }
    }
}




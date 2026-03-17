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

//code taken from CLI.cpp
std::string HashTable::lowerCase(const std::string& str) {
    std::string res = str;
    transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return res;
}


//utilized DJB hash: https://helloacm.com/the-simplest-string-hash-function-djb2-algorithm-and-implementations/
std::size_t HashTable::hashFunction(const std::string& key) {
    unsigned long hash = 5381;

    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % buckets.size();
}

double HashTable::loadFactor() const{
    if (buckets.empty()) {
        return 0.0;
    }
    return static_cast<double>(keys) / static_cast<double>(buckets.size());
}

//used this for implementation of rehash: https://www.geeksforgeeks.org/dsa/load-factor-and-rehashing/
void HashTable::rehash() {
    std::vector<Node*> temp = buckets;
    buckets.resize(2 * buckets.size());
    for (size_t i = 0; i < buckets.size(); i++) {
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

// used insert function here for inspiration: https://www.geeksforgeeks.org/dsa/implementation-of-hash-table-in-c-using-separate-chaining/
void HashTable::insert(const std::string& key, const FoodItem* food) {
    if (food == nullptr) {
        return;
    }
    std::string lowerKey = lowerCase(key);
    std::size_t bucketIndex = hashFunction(lowerKey);

    Node* head = buckets[bucketIndex];
    while (head != nullptr) {
        if (head->key == lowerKey) {
            head->values.push_back(food);
            return;
        }
        head = head->next;
    }
    Node* newNode = new Node(lowerKey, food);
    if (buckets[bucketIndex] == nullptr) {
        buckets[bucketIndex] = newNode;
    }
    else {
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
    }
    keys++;
    
    //Decided on keeping a max load factor of 0.8 like mentioned in lectures
    if (loadFactor() > 0.8) {
        rehash();
    }
}

std::vector<const FoodItem*> HashTable::find(const std::string& key) {
    if (buckets.empty()) {
        return {};
    }
    std::string lowerKey = lowerCase(key);
    std::size_t bucketIndex = hashFunction(lowerKey);

    Node* head = buckets[bucketIndex];

    while (head != nullptr) {
        if (head->key == lowerKey) {
            return head->values;
        }
        head = head->next;
    }
    return {};
}

void HashTable::buildFromFoods(const std::vector<FoodItem>& foods) {
    for (const auto& food : foods) {
        if (food.name.empty() == false) {
            insert(food.name, &food);
        }
    }
}

size_t HashTable::size() {
    return keys;
}

size_t HashTable::bucket_count() {
    return buckets.size();
}

bool HashTable::empty() {
    return size() == 0;
}




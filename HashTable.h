#include <string>
#include <cstddef>
#include <vector>
#include "FoodItem.h"

class HashTable {
    private:
    struct Node {
        std::string key;
        std::vector<const FoodItem*> values;
        Node* next;

        Node(const std::string& key, const FoodItem* food) : key(key), values{food}, next(nullptr) {}
    };

    std::vector<Node*> buckets;
    std::size_t keys;

    double loadFactor() const;
    void rehash();
    void clearOut();

    std::size_t hashFunction(const std::string& key);
    std::string lowerCase(const std::string& str);

    public:
    HashTable(size_t firstBucketCount = 10007);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void insert(const std::string& key, const FoodItem* food);
    std::vector<const FoodItem*> find(const std::string& key);

    void buildFromFoods(const std::vector<FoodItem>& foods);

    size_t size();
    size_t bucket_count();
    bool empty();
};
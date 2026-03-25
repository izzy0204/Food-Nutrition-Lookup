#include <iostream>
#include "Parser.h"
#include "CLI.h"
using namespace std;

int main() {
    vector<FoodItem> foods = loadFoodsFromTSV("../data/opennutrition_foods.tsv");

    cout << "Loaded " << foods.size() << " foods.\n";

    HashTable ht;
    ht.buildFromFoods(foods);

    RBT rbt;
    rbt.buildFromFoods(foods);

    runCLI(foods, ht, rbt);
    
    return 0;
}
#include <iostream>
#include "Parser.h"
#include "CLI.h"
using namespace std;

int main() {
    vector<FoodItem> foods = loadFoodsFromTSV("../data/opennutrition_foods.tsv");

    cout << "Loaded " << foods.size() << " foods.\n";

    runCLI(foods);
    //printSampleFoods(foods,5);
    return 0;
}
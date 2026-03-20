#ifndef PROJECT2B_FOODITEM_H
#define PROJECT2B_FOODITEM_H

#include <string>
using namespace std;

struct FoodItem {
    string id;
    string name;
    string type;
    string ingredients;
    string alternateNames;
    string serving;

    double calories = 0.0;
    double protein = 0.0;
    double carbs = 0.0;
    double fat = 0.0;
    double sodium = 0.0;
    double sugar = 0.0;
};

#endif
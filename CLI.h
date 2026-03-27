#ifndef PROJECT2B_CLI_H
#define PROJECT2B_CLI_H

#include <vector>
#include "FoodItem.h"

class HashTable;
class RBT;


void displayMenu();
void searchByName(HashTable& ht, RBT& rbt);
void filterByNutrition(const vector<FoodItem>& foods);
void excludeIngredient(const vector<FoodItem>& foods);
void allergenFilter(const vector<FoodItem>& foods);
void buildMeal(const vector<FoodItem>& foods);
void compareDataStructures(const vector<FoodItem>& foods);
void compareRepeatedLookups(const std::vector<FoodItem>& foods);
void runCLI(vector<FoodItem>& foods, HashTable& ht, RBT& rbt);

#endif
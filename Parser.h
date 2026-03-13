#ifndef PROJECT2B_PARSER_H
#define PROJECT2B_PARSER_H

#include <vector>
#include <string>
#include "FoodItem.h"
using namespace std;

vector<string> parseTSVRow(const string& line);

double extractNutrient(const string& nutritionStr, const string& key);

FoodItem buildFoodItem(const vector<string>& columns);

bool isValidFood(const FoodItem& food);

vector<FoodItem> loadFoodsFromTSV(const string& filename);

void printSampleFoods(const vector<FoodItem>& foods, int count);

#endif
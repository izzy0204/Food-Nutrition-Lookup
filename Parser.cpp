#include "Parser.h"
#include <fstream>
#include <iostream>
using namespace std;

vector<string> parseTSVRow(const string& line) {
    vector<string> fields;
    string current;

    for (char c : line) {
        if (c == '\t') {
            fields.push_back(current);
            current.clear();
        }
        else {
            current += c;
        }
    }

    fields.push_back(current);
    return fields;
}

double extractNutrient(const string& nutritionStr, const string& key) {
    string target = "\"" + key + "\":";
    size_t pos = nutritionStr.find(target);

    if (pos == string::npos) {
        return 0.0;
    }

    pos += target.length();

    while (pos < nutritionStr.length() && nutritionStr[pos] == ' ') {
        pos++;
    }

    size_t end = pos;
    while (end < nutritionStr.length() &&
           (isdigit(static_cast<unsigned char>(nutritionStr[end])) ||
            nutritionStr[end] == '.' ||
            nutritionStr[end] == '-')) {
        end++;
    }

    if (end == pos) {
        return 0.0;
    }

    return stod(nutritionStr.substr(pos, end - pos));
}

FoodItem buildFoodItem(const vector<string>& columns) {
    FoodItem food;

    food.id = columns[0];
    food.name = columns[1];
    food.alternateNames = columns[2];
    food.type = columns[4];
    food.serving = columns[6];
    food.ingredients = columns[11];

    const string& nutrition = columns[7];

    food.calories = extractNutrient(nutrition, "calories");
    food.protein = extractNutrient(nutrition, "protein");
    food.carbs = extractNutrient(nutrition, "carbohydrates");
    food.fat = extractNutrient(nutrition, "total_fat");
    food.sodium = extractNutrient(nutrition, "sodium");

    return food;
}

bool isValidFood(const FoodItem& food) {
    return !food.id.empty() && !food.name.empty();
}

vector<FoodItem> loadFoodsFromTSV(const string& filename) {
    vector<FoodItem> foods;
    foods.reserve(330000);
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening dataset\n";
        return foods;
    }

    string line;

    // skip header
    getline(file, line);

    while (getline(file, line)) {
        vector<string> columns = parseTSVRow(line);

        if (columns.size() < 13) {
            continue;
        }

        FoodItem food = buildFoodItem(columns);

        if (isValidFood(food)) {
            foods.push_back(food);
        }
    }

    file.close();
    return foods;
}

void printSampleFoods(const vector<FoodItem>& foods, int count) {
    for (int i = 0; i < count && i < static_cast<int>(foods.size()); i++) {
        cout << "\nFood " << i + 1 << "\n";
        cout << "Name: " << foods[i].name << "\n";
        cout << "Calories: " << foods[i].calories << "\n";
        cout << "Protein: " << foods[i].protein << "\n";
        cout << "Carbs: " << foods[i].carbs << "\n";
        cout << "Fat: " << foods[i].fat << "\n";
        cout << "Sodium: " << foods[i].sodium << "\n";
    }
}
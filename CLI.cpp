#include "CLI.h"
#include "RBT.h"
#include "HashTable.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <map>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

static string toLowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return tolower(c); });
    return text;
}

static void printFood(const FoodItem& food) {
    cout << "\nName: " << food.name;
    cout << "\nType: " << food.type;
    cout << "\nCalories: " << food.calories << " calories";
    cout << "\nProtein: " << food.protein << "g";
    cout << "\nCarbs: " << food.carbs << "g";
    cout << "\nFat: " << food.fat << "g";
    cout << "\nSodium: " << food.sodium << "mg";
    cout << "\nSugar: " << food.sugar << "g" << endl;
}

void displayMenu() {
    cout << "\n=== Food Nutrition Lookup ===\n";
    cout << "1. Load dataset\n";
    cout << "2. Search by name\n";
    cout << "3. Filter by nutrition\n";
    cout << "4. Exclude ingredient\n";
    cout << "5. Filter by allergen\n";
    cout << "6. Build simple meal\n";
    cout << "7. Compare data structures\n";
    cout << "8. Compare repeated lookups (average search time)\n";
    cout << "9. Exit\n";
    cout << "Enter choice: ";
}

void searchByName(HashTable& ht, RBT& rbt) {
    string query;
    cout << "\nEnter food name keyword: ";
    getline(cin, query);

    auto hashStart = high_resolution_clock::now();
    vector<const FoodItem*> hashResults = ht.find(query);
    auto hashEnd = high_resolution_clock::now();

    auto rbtStart = high_resolution_clock::now();
    vector<const FoodItem*> rbtResults = rbt.find(query);
    auto rbtEnd = high_resolution_clock::now();

    auto hashSearchTime = duration_cast<microseconds>(hashEnd - hashStart).count();
    auto rbtSearchTime = duration_cast<microseconds>(rbtEnd - rbtStart).count();

    cout << "=== Name Search Comparison in RBT and HashTable ===\n";
    cout << "Exact Query: " << query << "\n";
    cout << "HashTable search time: " << hashSearchTime << " microseconds.\n";
    cout << "Red-Black Tree search time: " << rbtSearchTime << " microseconds.\n";
    cout << " " << endl;
    cout << "HashTable matches: " << hashResults.size() << "\n";
    cout << "Red-Black Tree matches: " << rbtResults.size() << "\n";
    cout << "Same Number of Matches: " << (hashResults.size() == rbtResults.size() ? "Yes" : "No") << "\n";
    cout << " " << endl;
    cout << "Results: " << "\n";
    int blocker = 0;
    for (const auto* food : hashResults) {
        printFood(*food);
        blocker++;
        if (blocker == 20) break;
    }
    if (blocker == 0) cout << "No foods found.";
    else cout << "Displayed " << blocker << " results.\n";
}

void filterByNutrition(const vector<FoodItem>& foods) {
    double maxCalories, minProtein, maxCarbs, maxSodium, maxSugar;

    cout << "\nEnter maximum calories: ";
    cin >> maxCalories;

    cout << "Enter minimum protein (in grams): ";
    cin >> minProtein;

    cout << "Enter maximum carbs (in grams): ";
    cin >> maxCarbs;

    cout << "Enter maximum sodium (in milligrams): ";
    cin >> maxSodium;

    cout << "Enter maximum sugar (in grams): ";
    cin >> maxSugar;


    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int results = 0;

    for (const auto& food : foods) {
        if (food.calories <= maxCalories &&
            food.protein >= minProtein &&
            food.carbs <= maxCarbs &&
            food.sodium <= maxSodium &&
            food.sugar <= maxSugar){

            printFood(food);
            results++;
            if (results == 10) {
                break;
            }
        }
    }

    if (results == 0) {
        cout << "\nNo foods matched those nutrition constraints.\n";
    } else {
        cout << "\nDisplayed " << results << " result(s).\n";
    }
}

void excludeIngredient(const vector<FoodItem>& foods) {
    string keyword;
    cout << "\nEnter ingredient keyword to exclude: ";
    getline(cin, keyword);

    keyword = toLowerCase(keyword);
    int results = 0;

    for (const auto& food : foods) {
        if (food.ingredients.empty()) {
            continue;
        }

        string ingredients = toLowerCase(food.ingredients);

        if (ingredients.find(keyword) == string::npos) {
            cout << "\nName: " << food.name;
            cout << "\nType: " << food.type;
            cout << "\nIngredients: " << food.ingredients;
            cout << "\nCalories: " << food.calories << " calories";
            cout << "\nProtein: " << food.protein << "g";
            cout << "\nCarbs: " << food.carbs << "g";
            cout << "\nFat: " << food.fat << "g";
            cout << "\nSodium: " << food.sodium << "mg";
            cout << "\nSugar: " << food.sugar << "g" << endl;

            results++;
            if (results == 10) {
                break;
            }
        }
    }

    if (results == 0) {
        cout << "\nNo foods found after excluding that ingredient.\n";
    } else {
        cout << "\nDisplayed " << results << " result(s).\n";
    }
}

void allergenFilter(const vector<FoodItem>& foods) {
    map<int, vector<string>> allergenKeywords = {
        {1, {"milk", "butter", "cheese", "cream", "whey", "casein", "yogurt"}},
        {2, {"egg", "eggs", "albumin"}},
        {3, {"peanut", "groundnut"}},
        {4, {"soy", "soya", "soybean", "soy lecithin"}},
        {5, {"wheat", "gluten", "flour", "soy sauce"}},
        {6, {"tree nut", "almond", "cashew", "walnut", "pecan", "pistachio", "hazelnut"}}
    };

    cout << "\nChoose an allergen to avoid:\n";
    cout << "1. Dairy\n";
    cout << "2. Egg\n";
    cout << "3. Peanut\n";
    cout << "4. Soy\n";
    cout << "5. Wheat/Gluten\n";
    cout << "6. Tree Nuts\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (allergenKeywords.find(choice) == allergenKeywords.end()) {
        cout << "\nInvalid allergen choice.\n";
        return;
    }

    int results = 0;

    for (const auto& food : foods) {
        if (food.ingredients.empty()) {
            continue;
        }

        string ingredients = toLowerCase(food.ingredients);
        bool containsAllergen = false;

        for (const auto& keyword : allergenKeywords[choice]) {
            if (ingredients.find(keyword) != string::npos) {
                containsAllergen = true;
                break;
            }
        }

        if (!containsAllergen) {
            cout << "\nName: " << food.name;
            cout << "\nIngredients: " << food.ingredients;
            cout << "\nCalories: " << food.calories << " calories";
            cout << "\nProtein: " << food.protein << "g";
            cout << "\nCarbs: " << food.carbs << "g";
            cout << "\nFat: " << food.fat << "g";
            cout << "\nSodium: " << food.sodium << "mg";
            cout << "\nSugar: " << food.sugar << "g" << endl;

            results++;
            if (results == 10) {
                break;
            }
        }
    }

    if (results == 0) {
        cout << "\nNo foods found for that allergen filter.\n";
    } else {
        cout << "\nDisplayed " << results << " allergen-safe result(s) based on ingredient text.\n";
    }
}

void buildMeal(const vector<FoodItem>& foods) {
    double maxCalories, minProtein, maxCarbs, maxSodium, maxSugar;

    cout << "\nBuild a simple meal (protein + base + side).\n";
    cout << "Enter maximum total calories: ";
    cin >> maxCalories;

    cout << "Enter minimum total protein (in grams): ";
    cin >> minProtein;

    cout << "Enter maximum total carbs (in grams): ";
    cin >> maxCarbs;

    cout << "Enter maximum total sodium (in milligrams): ";
    cin >> maxSodium;

    cout << "Enter maximum total sugar (in grams): ";
    cin >>  maxSugar;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<const FoodItem*> proteins;
    vector<const FoodItem*> bases;
    vector<const FoodItem*> sides;

    for (const auto& food : foods) {
        string name = toLowerCase(food.name);

        if (food.calories <= 0) {
            continue;
        }

        if (name.find("chicken") != string::npos ||
            name.find("turkey") != string::npos ||
            name.find("tuna") != string::npos ||
            name.find("salmon") != string::npos ||
            name.find("beef") != string::npos ||
            name.find("pork") != string::npos ||
            name.find("egg") != string::npos ||
            name.find("tofu") != string::npos ||
            name.find("shrimp") != string::npos ||
            name.find("fish") != string::npos) {
            proteins.push_back(&food);
        }
        else if (name.find("rice") != string::npos ||
                 name.find("pasta") != string::npos ||
                 name.find("bread") != string::npos ||
                 name.find("potato") != string::npos ||
                 name.find("oatmeal") != string::npos ||
                 name.find("quinoa") != string::npos ||
                 name.find("noodle") != string::npos) {
            bases.push_back(&food);
        }
        else if (name.find("broccoli") != string::npos ||
                 name.find("spinach") != string::npos ||
                 name.find("salad") != string::npos ||
                 name.find("bean") != string::npos ||
                 name.find("carrot") != string::npos ||
                 name.find("vegetable") != string::npos ||
                 name.find("apple") != string::npos ||
                 name.find("banana") != string::npos ||
                 name.find("yogurt") != string::npos ||
                 name.find("milk") != string::npos) {
            sides.push_back(&food);
        }

        if (proteins.size() >= 20 && bases.size() >= 20 && sides.size() >= 20) {
            break;
        }
    }

    if (proteins.empty() || bases.empty() || sides.empty()) {
        cout << "\nNot enough categorized foods were found to build meals.\n";
        return;
    }

    int mealsFound = 0;

    for (const auto* protein : proteins) {
        for (const auto* base : bases) {
            for (const auto* side : sides) {
                double totalCalories = protein->calories + base->calories + side->calories;
                double totalProtein = protein->protein + base->protein + side->protein;
                double totalCarbs = protein->carbs + base->carbs + side->carbs;
                double totalSodium = protein->sodium + base->sodium + side->sodium;
                double totalSugar = protein->sugar + base->sugar + side->sugar;

                if (totalCalories <= maxCalories &&
                    totalProtein >= minProtein &&
                    totalCarbs <= maxCarbs &&
                    totalSodium <= maxSodium &&
                    totalSugar <= maxSugar) {

                    cout << "\nMeal Option " << mealsFound + 1 << ":\n";
                    cout << "Protein: " << protein->name << "\n";
                    cout << "Base: " << base->name << "\n";
                    cout << "Side: " << side->name << "\n";
                    cout << "Total Calories: " << totalCalories << " calories" << "\n";
                    cout << "Total Protein: " << totalProtein << "g" << "\n";
                    cout << "Total Carbs: " << totalCarbs << "g" << "\n";
                    cout << "Total Sodium: " << totalSodium << "mg" << "\n";
                    cout << "Total Sugar: " << totalSugar << "g" << "\n";

                    mealsFound++;
                    if (mealsFound == 5) {
                        cout << "\nDisplayed 5 meal option(s).\n";
                        return;
                    }
                }
            }
        }
    }

    if (mealsFound == 0) {
        cout << "\nNo meal combinations matched those constraints.\n";
    }
}


//Source: Execution time code from: https://www.geeksforgeeks.org/cpp/measure-execution-time-function-cpp/
void compareDataStructures(const vector<FoodItem>& foods) {
    string query;
    cout << "Enter exact food name to compare lookup performance: ";
    getline(cin, query);

    HashTable hashTable;
    RBT rbt;

    auto hashBuildStart = high_resolution_clock::now();
    hashTable.buildFromFoods(foods);
    auto hashBuildEnd = high_resolution_clock::now();

    auto rbtBuildStart = high_resolution_clock::now();
    rbt.buildFromFoods(foods);
    auto rbtBuildEnd = high_resolution_clock::now();

    auto hashSearchStart = high_resolution_clock::now();
    vector<const FoodItem*> hashResults = hashTable.find(query);
    auto hashSearchEnd = high_resolution_clock::now();

    auto rbtSearchStart = high_resolution_clock::now();
    vector<const FoodItem*> rbtResults = rbt.find(query);
    auto rbtSearchEnd = high_resolution_clock::now();

    auto hashBuildTime = duration_cast<microseconds>(hashBuildEnd - hashBuildStart).count();
    auto rbtBuildTime = duration_cast<microseconds>(rbtBuildEnd - rbtBuildStart).count();

    //nanoseconds needs to be used. microseconds will end up showing 0 for both.
    auto hashSearchTime = duration_cast<microseconds>(hashSearchEnd - hashSearchStart).count();
    auto rbtSearchTime = duration_cast<microseconds>(rbtSearchEnd - rbtSearchStart).count();

    cout << "=== Data Structure Comparison ===\n";
    cout << "Dataset: # of rows loaded from dataset: " << foods.size() << endl;
    cout << "RBT: # of nodes: " << rbt.size() << endl;
    cout << "Hashtable: # of unique names: " << hashTable.size() << endl;
    cout << "Hashtable: # of buckets: " << hashTable.bucket_count() << endl;
    cout << "Exact query: " << query << "\n";

    cout << "--- Build Times --- \n";
    cout << "Hash Table build time: " << hashBuildTime << " microseconds\n";
    cout << "Red-Black Tree build time: " << rbtBuildTime << " microseconds\n" << endl;

    cout << "--- Search Times --- \n";
    cout << "Hash Table search time: " << hashSearchTime << " microseconds\n";
    cout << "Red-Black Tree search time: " << rbtSearchTime << " microseconds\n" << endl;

    cout << "--- Match Counts ---\n";
    cout << "Hash Table matches found: " << hashResults.size() << "\n";
    cout << "Red-Black Tree matches found: " << rbtResults.size() << "\n" << endl;
}

void compareRepeatedLookups(const vector<FoodItem>& foods) {
    string query;
    cout << "Enter exact food name to compare repeated lookup performance: ";
    getline(cin, query);

    int repetitions;
    cout << "Enter number of repeated lookups: ";
    while (!(cin >> repetitions) || repetitions <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    HashTable hashTable;
    RBT rbt;

    auto hashBuildStart = high_resolution_clock::now();
    hashTable.buildFromFoods(foods);
    auto hashBuildEnd = high_resolution_clock::now();

    auto rbtBuildStart = high_resolution_clock::now();
    rbt.buildFromFoods(foods);
    auto rbtBuildEnd = high_resolution_clock::now();

    vector<const FoodItem*> hashResults;
    vector<const FoodItem*> rbtResults;

    auto hashSearchStart = high_resolution_clock::now();
    for (int i = 0; i < repetitions; i++) {
        hashResults = hashTable.find(query);
    }
    auto hashSearchEnd = high_resolution_clock::now();

    auto rbtSearchStart = high_resolution_clock::now();
    for (int i = 0; i < repetitions; i++) {
        rbtResults = rbt.find(query);
    }
    auto rbtSearchEnd = high_resolution_clock::now();

    auto hashBuildTime = duration_cast<microseconds>(hashBuildEnd - hashBuildStart).count();
    auto rbtBuildTime = duration_cast<microseconds>(rbtBuildEnd - rbtBuildStart).count();

    auto hashTotalSearchTime = duration_cast<microseconds>(hashSearchEnd - hashSearchStart).count();
    auto rbtTotalSearchTime = duration_cast<microseconds>(rbtSearchEnd - rbtSearchStart).count();

    double hashAverageSearchTime = static_cast<double>(hashTotalSearchTime) / repetitions;
    double rbtAverageSearchTime = static_cast<double>(rbtTotalSearchTime) / repetitions;

    cout << "=== Repeated Lookup Comparison ===\n";
    cout << "Dataset: # of rows loaded from dataset: " << foods.size() << endl;
    cout << "RBT: # of nodes: " << rbt.size() << endl;
    cout << "Hashtable: # of unique names: " << hashTable.size() << endl;
    cout << "Hashtable: # of buckets: " << hashTable.bucket_count() << endl;
    cout << "Exact query: " << query << "\n";
    cout << "Repetitions: " << repetitions << "\n\n";

    cout << "--- Build Times ---\n";
    cout << "Hash Table build time: " << hashBuildTime << " microseconds\n";
    cout << "Red-Black Tree build time: " << rbtBuildTime << " microseconds\n\n";

    cout << "--- Repeated Search Times ---\n";
    cout << "Hash Table total search time: " << hashTotalSearchTime << " microseconds\n";
    cout << "Red-Black Tree total search time: " << rbtTotalSearchTime << " microseconds\n\n";

    cout << "--- Average Search Times ---\n";
    cout << "Hash Table average search time: " << hashAverageSearchTime << " microseconds\n";
    cout << "Red-Black Tree average search time: " << rbtAverageSearchTime << " microseconds\n\n";

    cout << "--- Match Counts ---\n";
    cout << "Hash Table matches found: " << hashResults.size() << "\n";
    cout << "Red-Black Tree matches found: " << rbtResults.size() << "\n\n";

    if (hashAverageSearchTime < rbtAverageSearchTime) {
        cout << "Hash Table was faster on average for repeated exact lookups.\n";
    }
    else if (rbtAverageSearchTime < hashAverageSearchTime) {
        cout << "Red-Black Tree was faster on average for repeated exact lookups.\n";
    }
    else {
        cout << "Both structures had the same average search time.\n";
    }
}

void runCLI(vector<FoodItem>& foods, HashTable& ht, RBT& rbt) {
    int choice = 0;

    while (choice != 9) {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            cout << "\nDataset already loaded.\n";
            cout << "Total foods available: " << foods.size() << "\n";
        }
        else if (choice == 2) {
            searchByName(ht, rbt);
        }
        else if (choice == 3) {
            filterByNutrition(foods);
        }
        else if (choice == 4) {
            excludeIngredient(foods);
        }
        else if (choice == 5) {
            allergenFilter(foods);
        }
        else if (choice == 6) {
            buildMeal(foods);
        }
        else if (choice == 7) {
            compareDataStructures(foods);
        }
        else if (choice == 8) {
            compareRepeatedLookups(foods);
        }
        else if (choice == 9) {
            cout << "Goodbye\n";
        }
        else {
            cout << "Invalid choice. Please enter a number between 1 to 8." << endl;
            cout<< "If menu is not displaying, press enter.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
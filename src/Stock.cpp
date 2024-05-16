/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Stock
*/

#include "Stock.hpp"

plazza::Stock::Stock()
{
    _stockIngredients["dough"] = 5;
    _stockIngredients["tomato"] = 5;
    _stockIngredients["gruyere"] = 5;
    _stockIngredients["ham"] = 5;
    _stockIngredients["mushroom"] = 5;
    _stockIngredients["steak"] = 5;
    _stockIngredients["eggPlant"] = 5;
    _stockIngredients["goatCheese"] = 5;
    _stockIngredients["chiefLove"] = 5;
}

plazza::Stock::~Stock()
{
}

void plazza::Stock::refillStock(plazza::Stock stock, int time)
{
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        for (auto it = stock._stockIngredients.begin(); it != stock._stockIngredients.end(); it++) {
            if (it->second < 5)
                it->second++;
        }
    }
}

void plazza::Stock::takeFromStock(std::string ingredient)
{
    auto it = _stockIngredients.find(ingredient);
    if (it != _stockIngredients.end()) {
        it->second -= 1;
    }
}

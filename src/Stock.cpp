/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Stock
*/

#include "Stock.hpp"

plazza::Stock::Stock(std::vector<std::string> ingredientsList)
{
    for (auto &ingredient : ingredientsList)
        _stockIngredients[ingredient] = 5;
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

/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Stock
*/

#include "Stock.hpp"

plazza::Stock::Stock(std::vector<std::string> ingredientsList)
{
    for (auto &ingredient : ingredientsList) {
        _stockIngredients[ingredient] = 5;
    }
}

void plazza::Stock::refillStock(std::shared_ptr<plazza::Stock> stock, int time)
{
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        for (auto it = stock->_stockIngredients.begin(); it != stock->_stockIngredients.end(); it++) {
            if (it->second < 5)
                it->second++;
        }
        _stockCondVar.notify_all();
    }
}

void plazza::Stock::takeFromStock(std::string ingredient)
{
    auto it = _stockIngredients.find(ingredient);
    if (it != _stockIngredients.end()) {
        {
            std::unique_lock<std::mutex> lock(_stockMutex);
            _stockCondVar.wait(lock, [this, it] { return it->second > 0;});
            it->second -= 1;
        }
    }
}

std::map<std::string, std::size_t> plazza::Stock::getStock()
{
    return _stockIngredients;
}

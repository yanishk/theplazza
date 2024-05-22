/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Pizza
*/

#include "Pizza.hpp"
#include <memory>

void plazza::Pizza::setPizzaName(std::string &pizzaName)
{
    _pizzaName = pizzaName;
}

std::string plazza::Pizza::getPizzaName()
{
    return _pizzaName;
}

void plazza::Pizza::setCookingTime(std::size_t cookingTime)
{
    _cookingTime = cookingTime;
}

std::size_t plazza::Pizza::getCookingTime()
{
    return _cookingTime;
}

std::size_t plazza::Pizza::getIngredientNumber()
{
    return _margIngredients.size();
}

void plazza::Pizza::pushIngredient(std::string &ingredientName)
{
    _margIngredients.push_back(ingredientName);
}

void plazza::Pizza::cookPizza(const std::string pizzaSize, float multiplier, std::shared_ptr<plazza::Stock> stock, int millisecond)
{
    for (auto &it: _margIngredients) {
        stock->takeFromStock(it, millisecond);
    }
    int timeCooking = static_cast<float>((_cookingTime * 1000) * multiplier);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeCooking));
    std::cout << "One " << _pizzaName << " in size " << pizzaSize << " comin' right up" << std::endl;
}

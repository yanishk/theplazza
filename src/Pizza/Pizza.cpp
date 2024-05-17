/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Pizza
*/

#include "Pizza.hpp"

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

void plazza::Pizza::cookPizza(float multiplier, plazza::Stock &stock)
{
    for (auto &it: _margIngredients) {
        stock.takeFromStock(it);
    }
    int timeCooking = static_cast<float>((_cookingTime * 1000) * multiplier);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeCooking));
    std::cout << "One " << _pizzaName << " comin' right up" << std::endl;
}

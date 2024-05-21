/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include "Process.hpp"
#include <iostream>

plazza::Kitchen::Kitchen(int nbrCooks, int milliseconds, float multiplier, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenu, std::vector<std::string> ingredientsList) : _threadPool(nbrCooks), _millisecond(milliseconds),_stock(std::make_shared<plazza::Stock>(ingredientsList)), _multiplier(multiplier), _stockThread(_stock->refillStock, _stock, milliseconds)
{
    for (auto &pizza : plazzaMenu)
        _availablePizzas[pizza->getPizzaName()] = pizza;
}

plazza::Kitchen::~Kitchen()
{
    _stockThread.join();
}

void plazza::Kitchen::makePizza(std::string &pizzaName, std::string &pizzaSize)
{
    auto it = _availablePizzas.find(pizzaName);

    if (it == _availablePizzas.end())
        throw Kitchen::PizzaUnrecognized();
    std::shared_ptr<plazza::Pizza> pizza = it->second;
    _threadPool.putInQueue([this, pizza, pizzaSize] {
        pizza->cookPizza(pizzaSize, _multiplier, _stock, _millisecond);
    });
}

std::shared_ptr<plazza::Stock> plazza::Kitchen::getStockObject()
{
    return _stock;
}

int plazza::Kitchen::getPizzasInWork()
{
    return _threadPool.getWork();
}
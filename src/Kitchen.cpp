/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include "Process.hpp"
#include <iostream>

plazza::Kitchen::Kitchen(int nbrCooks, int milliseconds, float multiplier, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenu, std::vector<std::string> ingredientsList) : _threadPool(nbrCooks), _stock(ingredientsList), _multiplier(multiplier)
{
    plazza::Process process;
    pid_t pid = process.forkFunc();

    if (pid == 0) {
        std::thread stockThread(_stock.refillStock, _stock, milliseconds);
        stockThread.join();
    }

    for (auto &pizza : plazzaMenu)
        _availablePizzas[pizza->getPizzaName()] = pizza;
}

void plazza::Kitchen::makePizza(std::string &pizzaName)
{
    auto it = _availablePizzas.find(pizzaName);

    if (it == _availablePizzas.end())
        throw Kitchen::PizzaUnrecognized();
    std::shared_ptr<plazza::Pizza> pizza = it->second;
    _threadPool.putInQueue([this, pizza] {
        pizza->cookPizza(_multiplier, _stock);
    });
}

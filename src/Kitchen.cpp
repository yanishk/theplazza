/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include "Process.hpp"
#include "Margarita.hpp"
#include <iostream>

plazza::Kitchen::Kitchen(int nbrCooks, int milliseconds, float multiplier) : _threadPool(nbrCooks), _multiplier(multiplier)
{
    plazza::Process process;
    pid_t pid = process.forkFunc();

    if (pid == 0) {
        std::thread stockThread(_stock.refillStock, _stock, milliseconds);
        stockThread.join();
    }
    _availablePizzas["MARGARITA"] = std::make_shared<plazza::Margarita>();
}

void plazza::Kitchen::makePizza(std::string &pizzaName)
{
    auto it = _availablePizzas.find(pizzaName);

    if (it == _availablePizzas.end())
        throw Kitchen::PizzaUnrecognized();
    std::shared_ptr<plazza::IPizza> pizza = it->second;
    _threadPool.putInQueue([this, pizza] {
        pizza->cookPizza(_multiplier, _stock);
    });
}

/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include "Process.hpp"
#include <iostream>

plazza::Kitchen::Kitchen(int nbrCooks, int milliseconds, float multiplier, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenu, std::vector<std::string> ingredientsList) :
    _threadPool(nbrCooks), _millisecond(milliseconds), _stock(std::make_shared<plazza::Stock>(ingredientsList)),
    _multiplier(multiplier), _stockThread(_stock->refillStock, _stock, milliseconds),
    _clockT0(std::chrono::steady_clock::now()), _clockT1(std::chrono::steady_clock::now())
{
    for (auto &pizza : plazzaMenu)
        _availablePizzas[pizza->getPizzaName()] = pizza;
}

plazza::Kitchen::~Kitchen()
{
    _stockThread.join();
}

bool plazza::Kitchen::needToCloseKitchen()
{
    unsigned int msTimer = 5000;

    _clockT1 = std::chrono::steady_clock::now();
    auto timeElapsed = std::chrono::duration_cast
        <std::chrono::milliseconds>(_clockT1 - _clockT0);

    if (timeElapsed.count() > msTimer)
        return true;
    return false;
}

void plazza::Kitchen::makePizza(std::string &pizzaName, std::string &pizzaSize, std::size_t nbProcess)
{
    auto it = _availablePizzas.find(pizzaName);

    _clockT0 = std::chrono::steady_clock::now();
    _clockT1 = std::chrono::steady_clock::now();
    if (it == _availablePizzas.end())
        throw Kitchen::PizzaUnrecognized();
    _pizzaInCharge.push_back(std::make_pair(it->first, pizzaSize));
    _threadPool.putInQueue([this, it, pizzaSize, nbProcess] {
    it->second->cookPizza(pizzaSize, _multiplier, _stock, _millisecond, _pizzaInCharge, nbProcess);
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

int plazza::Kitchen::getAvailableCooks()
{
    return _threadPool.getAvailableThreads();
}

int plazza::Kitchen::getSpaceLeft()
{
    return _threadPool.getSpaceLeft();
}

void plazza::Kitchen::makeStatus(std::size_t nbProcess)
{
    std::cout << "Kitchen " << nbProcess << " {" <<  std::endl;
    std::map<std::string, std::size_t> map = getStockObject()->getStock();
    std::cout << "\tIngredients {" << std::endl;
    for (auto it = map.begin(); it != map.end(); it++)
        std::cout << "\t\t" << it->first << ": " << it->second << std::endl;
    std::cout << "\t}" << std::endl;
    std::cout << "\tNumber of pizzas in work: " << getPizzasInWork() << std::endl;
    std::cout << "\tNumber of available cooks: " << getAvailableCooks() << std::endl;
    std::cout << "\tSpace left for pizzas: " << getSpaceLeft() << std::endl;
    std::cout << "\tPizzas being worked on {" << std::endl;
    for (auto &it : _pizzaInCharge) {
        std::cout << "\t\t" << it.first << " " << it.second << std::endl;
    }
    std::cout << "\t}" << std::endl;
    std::cout << "}" << std::endl << std::endl;
}
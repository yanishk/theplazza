/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Margarita
*/

#include <iostream>
#include "Margarita.hpp"
#include "Stock.hpp"

plazza::Margarita::Margarita()
{
    _MargIngredients.push_back("dough");
    _MargIngredients.push_back("tomato");
    _MargIngredients.push_back("gruyere");
}

plazza::Margarita::Margarita::~Margarita()
{
}

void plazza::Margarita::cookPizza(float multiplier, plazza::Stock *stock) {
    for (auto &it: _MargIngredients) {
        stock->takeFromStock(it);
    }
    int timeCooking = (int)1000 * multiplier;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeCooking));
    std::cout << "One Margarita comin' right up" << std::endl;
};
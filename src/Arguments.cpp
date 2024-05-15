/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Arguments
*/

#include "Arguments.hpp"
#include <string>
#include <iostream>

plazza::Arguments::Arguments()
{
}

plazza::Arguments::~Arguments()
{
}

int plazza::Arguments::checkArgValidity() const
{
    if (_multiplier <= 0 || _nbrCooks <= 0 || _milliseconds <= 0) {
        std::cerr << "Arguments should be positivie values" << std::endl;
        return 84;
    }
    return 0;
}

int plazza::Arguments::retrieveArgs(const char **av)
{
    try {
        _multiplier = std::stof(av[1]);
        _nbrCooks = std::stoi(av[2]);
        _milliseconds = std::stoi(av[3]);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return checkArgValidity();
}

int plazza::Arguments::getNbrCooks() const noexcept
{
    return _nbrCooks;
}

int plazza::Arguments::getMilliseconds() const noexcept
{
    return _milliseconds;
}

float plazza::Arguments::getMultiplier() const noexcept
{
    return _multiplier;
}
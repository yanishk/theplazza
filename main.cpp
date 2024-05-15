/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** main
*/

#include <iostream>
#include <string>
#include "Arguments.hpp"
#include "Waiter.hpp"

int getHelp(int ac, const char **av)
{
    if (ac == 2 && av[1] == "--help") {
        std::cerr << "Usage: ./plazza <multiplier> <nbr_cooks> <milliseconds>" << std::endl;
        return 0;
    }
    if (ac != 4) {
        std::cerr << "Usage: ./plazza <multiplier> <nbr_cooks> <milliseconds>" << std::endl;
        return 84;
    }
    return 0;
}

int main(int ac, const char **av)
{
    plazza::Arguments args;
    plazza::Waiter reception;

    if (getHelp(ac, av) == 84)
        return 84;
    if (args.retrieveArgs(av) == 84)
        return 84;
    try {
        reception.placeOrders();
    } catch (const plazza::Waiter::ParsingCommandFailed &err) {
        std::cerr << err.what() << std::endl;
        return 84;
    }
    return 0;
}

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
    std::string firstArg;

    if (ac == 2)
        firstArg = av[1];
    if (ac == 2 && firstArg == "--help") {
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

    if (getHelp(ac, av) == 84)
        return 84;
    try {
        args.retrieveArgs(av);
        plazza::Waiter waiter;
        waiter.placeOrders(args);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        return 84;
    }
    return 0;
}

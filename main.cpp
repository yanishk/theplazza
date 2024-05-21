/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** main
*/

#include <iostream>
#include <string>
#include "Arguments.hpp"
#include "ThreadPool.hpp"
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

    if (getHelp(ac, av) == 84)
        return 84;
    try {
        args.retrieveArgs(av);
    } catch (std::exception &err) {
        std::cerr << err.what() << std::endl;
        return 84;
    }
    try {
        plazza::Waiter waiter;
        waiter.placeOrders(args);
    } catch(const plazza::PlazzaMenu::ParsingPlazzaMenuFailed &err) {
        std::cerr << err.what() << std::endl;
        return 84;
    }
    return 0;
}

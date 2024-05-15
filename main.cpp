#include <iostream>
#include <string>
#include "Arguments.cpp"

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

int main(int ac, const char **av) {
    plazza::Arguments args;

    if (getHelp(ac, av) == 84)
        return 84;
    if (args.retrieveArgs(av) == 84)
        return 84;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit")
            break;
    }
    return 0;
}

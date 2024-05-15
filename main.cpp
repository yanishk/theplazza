#include <iostream>
#include <string>

float multiplier;
int num_cooks;
int milliseconds;

int retrieveArgs(const char **av) {
    try {
        multiplier = std::stof(av[1]);
        num_cooks = std::stoi(av[2]);
        milliseconds = std::stoi(av[3]);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

int main(int ac, const char **av) {
    if (ac != 4) {
        std::cerr << "Usage: ./plazza <multiplier> <nbr_cooks> <milliseconds>" << std::endl;
        return 84;
    }
    if (retrieveArgs(av) == 84)
        return 84;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit")
            break;
    }
    return 0;
}

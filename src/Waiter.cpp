/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Waiter
*/

#include <thread>
#include <algorithm>

#include "Waiter.hpp"
#include "Process.hpp"
#include "Arguments.hpp"
#include "Kitchen.hpp"

plazza::Waiter::Waiter()
{
    for (auto &pizza : _menu.getMenu())
        _pizzaTypeVector.push_back(pizza->getPizzaName());
    for (auto &pizzaSize : _menu.getPizzaSizes())
        _pizzaSizeVector.push_back(pizzaSize);
}

void plazza::Waiter::resetCommandList()
{
    _tmpCommandsVector.clear();
    _commandsVector.clear();
}

void plazza::Waiter::checkCommandEnd(const std::string &line)
{
    std::size_t pos = line.find_first_not_of(" \t\n");

    if (pos != std::string::npos)
        throw Waiter::ParsingCommandFailed("Invalid number of arguments");
}

bool plazza::Waiter::isDelim(std::string &line)
{
    std::size_t pos = line.find_first_not_of(" \t\n");

    if (pos == std::string::npos) {
        if (line.length() != 0)
            line.clear();
        return false;
    }
    line.erase(0, pos);
    return true;
}

void plazza::Waiter::parseWord(std::string &line, std::vector<std::string> &commandInfos, const std::size_t &wordIndex)
{
    std::string word;
    std::size_t pos = line.find_first_of(" \n\t");

    if (pos == std::string::npos && wordIndex != _commandArguments - 1)
        throw Waiter::ParsingCommandFailed("Invalid command");
    word = line.substr(0, pos);
    line.erase(0, pos);
    commandInfos.push_back(word);
}

std::vector<std::string> plazza::Waiter::parseCommand(std::string &line)
{
    std::vector<std::string> commandInfos;

    for (std::size_t i = 0; i != _commandArguments; i++) {
        if (!isDelim(line) && i != 0)
            throw Waiter::ParsingCommandFailed("Invalid number of arguments");
        parseWord(line, commandInfos, i);
    }
    checkCommandEnd(line);
    if (commandInfos.size() != 3)
        throw Waiter::ParsingCommandFailed("Invalid number of arguments");
    return commandInfos;
}

void plazza::Waiter::printTmpCommands()
{
    for (auto &command : _tmpCommandsVector) {
        for (auto &word : command) {
            std::cout << word << std::endl;
        }
    }
}

void plazza::Waiter::printCommands()
{
    for (auto &command : _commandsVector) {
        std::cout << std::get<0>(command) << std::endl;
        std::cout << std::get<1>(command)<< std::endl;
        std::cout << std::get<2>(command)<< std::endl;
    }
}

void plazza::Waiter::upperToLowerCase(std::string &word)
{
    for (std::size_t i = 0; i != word.length(); i++)
        word[i] = std::tolower(word[i]);
}

void plazza::Waiter::lowertoUpperCase(std::string &word)
{
    for (std::size_t i = 0; i != word.length(); i++)
        word[i] = std::toupper(word[i]);
}

void plazza::Waiter::changeCommandsCase()
{
    for (auto &command : _tmpCommandsVector) {
        for (auto &word : command) {
            upperToLowerCase(word);
        }
    }
}

bool isPizzaNumber(std::string &line)
{
    std::size_t pos = line.find_first_of('x');

    if (pos != 0)
        return false;
    if (std::atoi(line.substr(pos + 1, line.length()).c_str()) == 0)
        return false;
    return true;
}

void plazza::Waiter::checkCommands()
{
    for (auto &command : _tmpCommandsVector) {
        if (std::find(_pizzaTypeVector.begin(), _pizzaTypeVector.end(), command.at(0)) == _pizzaTypeVector.end())
            throw Waiter::ParsingCommandFailed("Chosen pizza type doesn't exist");
        if (std::find(_pizzaSizeVector.begin(), _pizzaSizeVector.end(), command.at(1)) == _pizzaSizeVector.end())
            throw Waiter::ParsingCommandFailed("Chosen pizza size doesn't exist");
        if (!isPizzaNumber(command.at(2)))
            throw Waiter::ParsingCommandFailed("Number of pizza(s) not specified");
    }
}

void plazza::Waiter::fillCommands()
{

    for (auto &command : _tmpCommandsVector) {
        lowertoUpperCase(command.at(1));
        _commandsVector.push_back(std::make_tuple(command.at(0), command.at(1), std::atoi(command.at(2).substr(1, command.at(2).length()).c_str())));
    }
}

void plazza::Waiter::parseMultipleCommands(std::string &line)
{
    std::size_t pos;
    std::string command;

    while (pos != std::string::npos) {
        pos = line.find_first_of(";");
        command = line.substr(0, pos);
        line.erase(0, pos + 1);
        _tmpCommandsVector.push_back(parseCommand(command));
    }
    changeCommandsCase();
    checkCommands();
    fillCommands();
}

std::vector<std::tuple<std::string, std::string, std::size_t>> plazza::Waiter::getCommands()
{
    return _commandsVector;
}

void plazza::Waiter::placeOrders(plazza::Arguments args)
{
    std::string line;
    plazza::Kitchen kitchen(args.getNbrCooks(), args.getMilliseconds(),
        args.getMultiplier(), _menu.getMenu(),_menu.getIngredients());

    while (std::getline(std::cin, line)) {
        if (line.length() == 0)
            continue;
        if (line == "status") {
            kitchen.makeStatus();
            continue;
        }
        try {
            parseMultipleCommands(line);
        } catch (const plazza::Waiter::ParsingCommandFailed &err) {
            std::cerr << err.what() << std::endl;
        }
        try {
            for (auto &command : _commandsVector) {
                for (std::size_t nbPizza = std::get<2>(command); nbPizza != 0; nbPizza--) {
                    kitchen.makePizza(std::get<0>(command), std::get<1>(command));
                }
            }
        } catch (const plazza::Kitchen::PizzaUnrecognized &err) {
            std::cerr << err.what() << std::endl;
        }
        resetCommandList();
    }
}

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

plazza::Waiter::Waiter()
{
    _pizzaTypeVector.push_back("REGINA");
    _pizzaTypeVector.push_back("MARGARITA");
    _pizzaTypeVector.push_back("AMERICANA");
    _pizzaTypeVector.push_back("FANTASIA");
    _pizzaSizeVector.push_back("S");
    _pizzaSizeVector.push_back("M");
    _pizzaSizeVector.push_back("L");
    _pizzaSizeVector.push_back("XL");
    _pizzaSizeVector.push_back("XXL");
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
        throw Waiter::ParsingCommandFailed("Invalid number of arguments");
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
        std::cout << command.first << std::endl;
        std::cout << command.second << std::endl;
    }
}

void plazza::Waiter::lowerToUpperCase(std::string &word)
{
    for (std::size_t i = 0; i != word.length(); i++)
        word[i] = std::toupper(word[i]);
}

void plazza::Waiter::changeCommandsCase()
{
    for (auto &command : _tmpCommandsVector) {
        for (auto &word : command) {
            lowerToUpperCase(word);
        }
    }
}

bool isPizzaNumber(std::string &line)
{
    std::size_t pos = line.find_first_of('X');

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
    for (auto &command : _tmpCommandsVector)
        _commandsVector.push_back(std::make_pair(command.at(0), std::atoi(command.at(2).substr(1, command.at(2).length()).c_str())));
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
    printCommands();
}

void plazza::Waiter::placeOrders(plazza::Stock stock, plazza::Arguments args)
{
    std::string line;
    plazza::Process process;
    pid_t pid = process.forkFunc();

    plazza::ThreadPool pool(args.getNbrCooks());
    if (pid == 0) {
        std::thread stockThread(stock.refillStock, stock, args.getMilliseconds());
        stockThread.join();
    } else {
        while (std::getline(std::cin, line)) {
            if (line == "exit")
                break;
            try {
                parseMultipleCommands(line);
            } catch (const plazza::Waiter::ParsingCommandFailed &err) {
                std::cerr << err.what() << std::endl;
            }
            if (line == "Margarita")
                pool.putInQueue([] { std::cout << "Margarita" << std::endl; });
            resetCommandList();
        }
    }
}

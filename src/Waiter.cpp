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

plazza::Waiter::Waiter() : _pipefdsVector(1024)
{
    fdID = 1;
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

void plazza::Waiter::forkKitchen(int processNum, int msgrid)
{
    pid_t pid = fork();

    if (pid != 0) {
        key_t keyReception = 99;
        int nbCook = 0;
        while (1) {
            struct my_msg some_data;
            struct my_msg new_some_data;
            msgrcv(processNum,(void *)&some_data,sizeof(&some_data),0,0);
            if (some_data.nbCook != -1)
                nbCook++;
            std::cout << RED << "Data received: \"" << some_data.nbCook << "\" on process : " << pid << " cook : " << nbCook << RESET << std::endl;
            some_data.nbCook = nbCook;
            new_some_data.nbCook = nbCook;
            std::cout << RED << "nb cook : " << some_data.nbCook << " test : " << some_data.test << RESET << std::endl;
            msgsnd(msgrid, (void *)&new_some_data, sizeof(&new_some_data), IPC_NOWAIT);
        }
    }
}

std::vector<key_t> plazza::Waiter::IPC(std::tuple<std::string, std::string, std::size_t> &command, std::vector<key_t> processVector)
{
    int msgrid;
    int kitchenID = 0;
    key_t keyReception = 99;
    struct my_msg some_data;
    struct my_msg new_some_data;
    bool createProcess = true;
    msgrid=msgget(keyReception, 0666|IPC_CREAT);

    for (std::size_t i = 0; i < processVector.size(); i++) {
        some_data.nbCook = -1;
        msgsnd(processVector.at(i), (void *)&some_data, sizeof(&some_data), IPC_NOWAIT);
        msgrcv(msgrid,(void *)&new_some_data,sizeof(&new_some_data),0,0);
        std::cout << GREEN << "cook1 = " << new_some_data.nbCook << RESET << std::endl;
        if (new_some_data.nbCook >= 5)
            createProcess = true;
        else {
            createProcess = false;
            kitchenID = processVector.back();
            break;
        }
    }

    if (createProcess) {
        key_t newKey = processVector.size() + 100;
        int processID = msgget(newKey, 0666|IPC_CREAT);
        std::cout << "pass, new key : " << newKey << std::endl;
        forkKitchen(processID, msgrid);
        some_data.nbCook = 0;
        processVector.push_back(processID);
        kitchenID=processVector.back();
    }
    some_data.nbCook = 0;
    some_data.test = 2;
    msgsnd(kitchenID, (void *)&some_data, sizeof(&some_data), IPC_NOWAIT);

    msgrcv(msgrid,(void *)&new_some_data,sizeof(&new_some_data),0,0);
    std::cout << GREEN << "cook2 = " << new_some_data.nbCook << RESET << std::endl;

    std::cout << "ID: " << kitchenID << std::endl;
    return processVector;
}

void plazza::Waiter::placeOrders(plazza::Arguments args)
{
    std::string line;
    plazza::Kitchen kitchen(args.getNbrCooks(), args.getMilliseconds(),
        args.getMultiplier(), _menu.getMenu(),_menu.getIngredients());
    nbrCooks = args.getNbrCooks();

    std::vector<int> processVector;

    while (std::getline(std::cin, line)) {
        if (line.length() == 0)
            continue;
        if (line == "status") {
            std::map<std::string, std::size_t> map = kitchen.getStockObject()->getStock();
            for (auto it = map.begin(); it != map.end(); it++)
                std::cout << it->first << ": " << it->second << std::endl;
            std::cout << "Number of pizzas in work: " << kitchen.getPizzasInWork() << std::endl;
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
                    // kitchen.makePizza(std::get<0>(command), std::get<1>(command));
                    processVector = IPC(command, processVector);
                }
            }
        } catch (const plazza::Kitchen::PizzaUnrecognized &err) {
            std::cerr << err.what() << std::endl;
        }
        resetCommandList();
    }
}
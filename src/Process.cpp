/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Process
*/

#include "Process.hpp"
#include "Kitchen.hpp"
#include "PizzaInfo.hpp"

plazza::Process::Process(plazza::Arguments args, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenuVector, std::vector<std::string> plazzaIngredientsVector) :
    _nbProcess(0), _args(args), _plazzaMenuVector(plazzaMenuVector), _plazzaIngredientsVector(plazzaIngredientsVector)
{
    key_t keyReception = ftok("./src/Process.cpp", 1);
    if (keyReception == -1)
        throw plazza::Process::FunctionFailure("ftok");
    _msgrid = msgget(keyReception, 0666 | IPC_CREAT);
}

pid_t plazza::Process::forkFunc()
{
    return fork();
}

void plazza::Process::forkKitchen(int processNum)
{
    pid_t pid = forkFunc();
    struct my_msg receiverKitchen;
    struct my_msg senderKitchen;

    std::memset(&senderKitchen.msg, 0, sizeof(&senderKitchen.msg));
    if (pid == -1)
        throw plazza::Process::FunctionFailure("fork");
    if (pid == 0) {
        plazza::Kitchen kitchen(_args.getNbrCooks(), _args.getMilliseconds(),
            _args.getMultiplier(), _plazzaMenuVector, _plazzaIngredientsVector);
        while (1) {
            if (msgrcv(processNum, &receiverKitchen, sizeof(receiverKitchen), 0, 0) == -1)
                throw plazza::Process::FunctionFailure("msgrcv");
            if (std::strcmp(receiverKitchen.msg, "status") == 0)
                kitchen.makeStatus(_nbProcess);
            else if (kitchen.getSpaceLeft() > 0 && std::strcmp(receiverKitchen.msg, "nbCooks") != 0) {
                plazza::PizzaInfo pizza;
                receiverKitchen.msg >> pizza;
                std::string pizzaTypeString = pizza.getPizzaTypeString();
                std::string pizzaSizeString = pizza.getPizzaSizeString();
                kitchen.makePizza(pizzaTypeString, pizzaSizeString);
            }
            if (kitchen.needToCloseKitchen() && kitchen.getSpaceLeft() == _args.getNbrCooks() * 2) {
                std::strcpy(senderKitchen.msg, "close");
                if (msgsnd(_msgrid, &senderKitchen, sizeof(senderKitchen), IPC_NOWAIT) == -1)
                    throw plazza::Process::FunctionFailure("msgsnd");
                break;
            }
            senderKitchen.nbCook = kitchen.getSpaceLeft();
            std::strcpy(senderKitchen.msg, "received");
            if (msgsnd(_msgrid, &senderKitchen, sizeof(senderKitchen), IPC_NOWAIT) == -1)
                throw plazza::Process::FunctionFailure("msgsnd");
        }
    }
}

void plazza::Process::createNewProcess(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString)
{
    int kitchenReceiverID = 0;

    _nbProcess += 1;
    key_t newKey = ftok("./src/Waiter.cpp", _nbProcess);
    if (newKey == -1)
        throw plazza::Process::FunctionFailure("ftok");
    kitchenReceiverID = msgget(newKey, 0666 | IPC_CREAT);
    forkKitchen(kitchenReceiverID);
    _processVector.push_back(kitchenReceiverID);
    std::strcpy(senderReception.msg, pizzaInfoString.c_str());
    if (msgsnd(kitchenReceiverID, &senderReception, sizeof(senderReception), IPC_NOWAIT) == -1)
        throw plazza::Process::FunctionFailure("msgsnd");
    if (msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0) == -1)
        throw plazza::Process::FunctionFailure("msgrcv");
}

void plazza::Process::getStatus(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString)
{
    for (std::size_t i = 0; i < _processVector.size(); i++) {
        std::strcpy(senderReception.msg, pizzaInfoString.c_str());
        senderReception.nbCook = 0;
        if (msgsnd(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT) == -1)
            throw plazza::Process::FunctionFailure("msgsnd");
        if (msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0) == -1)
            throw plazza::Process::FunctionFailure("msgrcv");
        if (std::strcmp(receiverReception.msg, "close") == 0) {
            if (msgctl(_processVector.at(i),IPC_RMID,NULL) == -1)
                throw plazza::Process::FunctionFailure("msgctl");
            _processVector.erase(_processVector.begin() + i);
            i--;
        }
    }
}

void plazza::Process::sendPizza(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString)
{
    std::pair<std::size_t, std::size_t> mostAvailKitchen{0, 0};

    for (std::size_t i = 0; i < _processVector.size(); i++) {
        std::strcpy(senderReception.msg, "nbCooks");
        if (msgsnd(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT) == -1)
            throw plazza::Process::FunctionFailure("msgsnd");
        if (msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0) == -1)
            throw plazza::Process::FunctionFailure("msgrcv");
        if (receiverReception.nbCook > mostAvailKitchen.second) {
            mostAvailKitchen.first = i;
            mostAvailKitchen.second = receiverReception.nbCook;
        } else if (std::strcmp(receiverReception.msg, "close") == 0) {
            if (msgctl(_processVector.at(i),IPC_RMID,NULL) == -1)
                throw plazza::Process::FunctionFailure("msgctl");
            _processVector.erase(_processVector.begin() + i);
            i--;
        }
    }
    if (mostAvailKitchen.second <= 0)
        createNewProcess(senderReception, receiverReception, pizzaInfoString);
    else {
        std::strcpy(senderReception.msg, pizzaInfoString.c_str());
        if (msgsnd(_processVector.at(mostAvailKitchen.first), &senderReception, sizeof(senderReception), IPC_NOWAIT) == -1)
            throw plazza::Process::FunctionFailure("msgsnd");
        if (msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0) == -1)
            throw plazza::Process::FunctionFailure("msgrcv");
    }
}

void plazza::Process::IPC(std::string &pizzaInfoString)
{
    struct my_msg senderReception;
    struct my_msg receiverReception;

    if (pizzaInfoString != "status") {
        sendPizza(senderReception, receiverReception, pizzaInfoString);
    } else {
        getStatus(senderReception, receiverReception, pizzaInfoString);
    }
}

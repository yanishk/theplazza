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

    if (pid == -1)
        throw plazza::Process::FunctionFailure("fork");
    if (pid == 0) {
        plazza::Kitchen kitchen(_args.getNbrCooks(), _args.getMilliseconds(),
            _args.getMultiplier(), _plazzaMenuVector, _plazzaIngredientsVector);
        while (1) {
            msgrcv(processNum, &receiverKitchen, sizeof(receiverKitchen), 0, 0);
            if (std::strcmp(receiverKitchen.msg, "status") == 0)
                kitchen.makeStatus();
            else if (kitchen.getSpaceLeft() > 0  && std::strcmp(receiverKitchen.msg, "nbCooks") != 0) {
                plazza::PizzaInfo pizza;
                receiverKitchen.msg >> pizza;
                std::string pizzaTypeString = pizza.getPizzaTypeString();
                std::string pizzaSizeString = pizza.getPizzaSizeString();
                kitchen.makePizza(pizzaTypeString, pizzaSizeString);
            }
            senderKitchen.nbCook = kitchen.getSpaceLeft();
            msgsnd(_msgrid, &senderKitchen, sizeof(senderKitchen), IPC_NOWAIT);
        }
    }
}

void plazza::Process::createNewProcess(struct my_msg senderReception, struct my_msg receiverReception, std::string pizzaInfoString)
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
    msgsnd(kitchenReceiverID, &senderReception, sizeof(senderReception), IPC_NOWAIT);
    msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
}

void plazza::Process::IPC(std::string pizzaInfoString)
{
    struct my_msg senderReception;
    struct my_msg receiverReception;
    bool createProcess = true;
    std::pair<std::size_t, std::size_t> mostAvailKitchen{0, 0};

    if (pizzaInfoString != "status") {
        for (std::size_t i = 0; i < _processVector.size(); i++) {
            std::strcpy(senderReception.msg, "nbCooks");
            msgsnd(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT);
            msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
            if (receiverReception.nbCook > mostAvailKitchen.second) {
                mostAvailKitchen.first = i;
                mostAvailKitchen.second = receiverReception.nbCook;
            }
        }
        if (mostAvailKitchen.second <= 0)
            createNewProcess(senderReception, receiverReception, pizzaInfoString);
        else {
            std::strcpy(senderReception.msg, pizzaInfoString.c_str());
            msgsnd(_processVector.at(mostAvailKitchen.first), &senderReception, sizeof(senderReception), IPC_NOWAIT);
            msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
        }
    } else {
        for (std::size_t i = 0; i < _processVector.size(); i++) {
            std::strcpy(senderReception.msg, pizzaInfoString.c_str());
            senderReception.nbCook = 0;
            msgsnd(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT);
            msgrcv(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
        }
    }
}

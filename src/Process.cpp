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
    key_t keyReception = CFunction::ftokFunc("./src/Process.cpp", 1);
    _msgrid = CFunction::msggetFunc(keyReception, 0666 | IPC_CREAT);
}

void plazza::Process::forkKitchen(int processNum)
{
    pid_t pid = CFunction::forkFunc();
    struct my_msg receiverKitchen;
    struct my_msg senderKitchen;

    std::memset(&senderKitchen.msg, 0, sizeof(&senderKitchen.msg));
    if (pid == 0) {
        plazza::Kitchen kitchen(_args.getNbrCooks(), _args.getMilliseconds(),
            _args.getMultiplier(), _plazzaMenuVector, _plazzaIngredientsVector);
        while (1) {
            CFunction::msgrcvFunc(processNum, &receiverKitchen, sizeof(receiverKitchen), 0, 0);
            if (std::strcmp(receiverKitchen.msg, "status") == 0)
                kitchen.makeStatus(_nbProcess);
            else if (kitchen.getSpaceLeft() > 0 && std::strcmp(receiverKitchen.msg, "nbCooks") != 0) {
                plazza::PizzaInfo pizza;
                receiverKitchen.msg >> pizza;
                std::string pizzaTypeString = pizza.getPizzaTypeString();
                std::string pizzaSizeString = pizza.getPizzaSizeString();
                kitchen.makePizza(pizzaTypeString, pizzaSizeString, _nbProcess);
            }
            if (kitchen.needToCloseKitchen() && kitchen.getSpaceLeft() == _args.getNbrCooks() * 2) {
                std::strcpy(senderKitchen.msg, "close");
                CFunction::msgsndFunc(_msgrid, &senderKitchen, sizeof(senderKitchen), IPC_NOWAIT);
                break;
            }
            senderKitchen.nbCook = kitchen.getSpaceLeft();
            std::strcpy(senderKitchen.msg, "received");
            CFunction::msgsndFunc(_msgrid, &senderKitchen, sizeof(senderKitchen), IPC_NOWAIT);
        }
    }
}

void plazza::Process::createNewProcess(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString)
{
    int kitchenReceiverID = 0;

    _nbProcess += 1;
    key_t newKey = CFunction::ftokFunc("./src/Waiter.cpp", _nbProcess);
    kitchenReceiverID = CFunction::msggetFunc(newKey, 0666 | IPC_CREAT);
    forkKitchen(kitchenReceiverID);
    _processVector.push_back(kitchenReceiverID);
    std::strcpy(senderReception.msg, pizzaInfoString.c_str());
    CFunction::msgsndFunc(kitchenReceiverID, &senderReception, sizeof(senderReception), IPC_NOWAIT);
    CFunction::msgrcvFunc(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
}

void plazza::Process::getStatus(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString)
{
    for (std::size_t i = 0; i < _processVector.size(); i++) {
        std::strcpy(senderReception.msg, pizzaInfoString.c_str());
        senderReception.nbCook = 0;
        CFunction::msgsndFunc(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT);
        CFunction::msgrcvFunc(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
        if (std::strcmp(receiverReception.msg, "close") == 0) {
            CFunction::msgctlFunc(_processVector.at(i),IPC_RMID,NULL);
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
        CFunction::msgsndFunc(_processVector.at(i), &senderReception, sizeof(senderReception), IPC_NOWAIT);
        CFunction::msgrcvFunc(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
        if (std::strcmp(receiverReception.msg, "close") == 0) {
            CFunction::msgctlFunc(_processVector.at(i),IPC_RMID,NULL);
            _processVector.erase(_processVector.begin() + i);
            i--;
        } else if (receiverReception.nbCook > mostAvailKitchen.second) {
            mostAvailKitchen.first = i;
            mostAvailKitchen.second = receiverReception.nbCook;
        }
    }
    if (mostAvailKitchen.second <= 0)
        createNewProcess(senderReception, receiverReception, pizzaInfoString);
    else {
        std::strcpy(senderReception.msg, pizzaInfoString.c_str());
        CFunction::msgsndFunc(_processVector.at(mostAvailKitchen.first), &senderReception, sizeof(senderReception), IPC_NOWAIT);
        CFunction::msgrcvFunc(_msgrid, &receiverReception, sizeof(receiverReception), 0, 0);
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

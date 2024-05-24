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

pid_t plazza::Process::forkFunc() {
    return fork();
}

void plazza::Process::forkKitchen(int processNum, int msgrid)
{
    pid_t pid = forkFunc();

    if (pid != 0) {
        plazza::Kitchen kitchen(_args.getNbrCooks(), _args.getMilliseconds(),
            _args.getMultiplier(), _menu.getMenu(),_menu.getIngredients());
        key_t keyReception = 99;
        int nbCook = 0;
        int all_pass = 0;
        while (1) {
            struct my_msg some_data;
            struct my_msg new_some_data;
            msgrcv(processNum,(void *)&some_data,sizeof(&some_data),0,0);
            all_pass++;
            if (std::strcmp(some_data.msg, "status") == 0) {
                kitchen.makeStatus();
                new_some_data.nbCook = 0;
                msgsnd(msgrid, (void *)&new_some_data, sizeof(&new_some_data), IPC_NOWAIT);
                std::cout << std::endl;
                continue;
            }
            if (std::strcmp(some_data.msg, "freeSp") == 0) {
                new_some_data.nbCook = kitchen.getAvailableCooks();
                msgsnd(msgrid, (void *)&new_some_data, sizeof(&new_some_data), IPC_NOWAIT);
                continue;
            }
            if (kitchen.getAvailableCooks() > 0) {
                nbCook++;
                plazza::PizzaInfo pizza;
                some_data.msg >> pizza;
                std::string pizzaTypeString = pizza.getPizzaTypeString();
                std::string pizzaSizeString = pizza.getPizzaSizeString();
                kitchen.makePizza(pizzaTypeString, pizzaSizeString);
            }

            new_some_data.nbCook = kitchen.getAvailableCooks();
            msgsnd(msgrid, (void *)&new_some_data, sizeof(&new_some_data), IPC_NOWAIT);
        }
    }
}

void plazza::Process::createNewProcess(bool createProcess, std::string pizzaInfoString)
{
    if (!createProcess || pizzaInfoString.compare("status") == 0 || pizzaInfoString.compare("freeSp") == 0)
        return;
    _nbProcess++;
    key_t newKey = ftok("./src/Waiter.cpp", _nbProcess);
    if (newKey == -1) {
        std::cerr << "faile to create process" << std::endl;
    }
    int processID = 0;
    processID = msgget(newKey, 0666 | IPC_CREAT);
    forkKitchen(processID, _msgrid);
    _processVector.push_back(processID);
    IPC(pizzaInfoString);
}

void plazza::Process::IPC(std::string pizzaInfoString)
{
    int kitchenID = 0;
    struct my_msg some_data;
    struct my_msg new_some_data;
    bool createProcess = true;

    for (std::size_t i = 0; i < _processVector.size(); i++) {
        std::strcpy(some_data.msg, pizzaInfoString.c_str());
        some_data.nbCook = 1;
        msgsnd(_processVector.at(i), (void *)&some_data, sizeof(&some_data), IPC_NOWAIT);
        msgrcv(_msgrid,(void *)&new_some_data,sizeof(&new_some_data),0,0);
        if (new_some_data.nbCook <= 0)
            createProcess = true;
        else {
            createProcess = false;
            kitchenID = _processVector.at(i);
            break;
        }
    }

    createNewProcess(createProcess, pizzaInfoString);
}

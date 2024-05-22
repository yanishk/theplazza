/*
** EPITECH PROJECT, 2024
** B-CCP-400-PAR-4-1-theplazza-yanis.harkouk
** File description:
** IPC
*/

#include "IPC.hpp"

void IPC::ipc(int processNum)
{
    pid_t pid = fork();

    if (pid != 0) {
        int msgid;
        int msgrid;
        struct my_msg some_data;
        key_t key = processNum;
        key_t keyReception = 99;
        msgid=msgget(key, 0666|IPC_CREAT);
        msgrid=msgget(keyReception, 0666|IPC_CREAT);
        int nbCook = 0;
        while (1) {
            msgrcv(msgid,(void *)&some_data,MAX_TEXT,0,0);
            nbCook++;
            std::cout << RED << "Data received: \"" << some_data.some_text << " on process : " << pid << " cook : " << nbCook << RESET << std::endl;
            some_data.nbCook = nbCook;
            msgsnd(msgrid, (void *)&some_data, MAX_TEXT, IPC_NOWAIT);
        }
    }
}

std::vector<key_t> IPC::comm(std::string line, std::vector<key_t> processVector)
{
    int msgrid;
    int kitchenID = 0;
    key_t keyReception = 99;
    struct my_msg some_data;
    bool createProcess = true;
    msgrid=msgget(keyReception, 0666|IPC_CREAT);
    for (int i = 0; i < processVector.size(); i++) {
        msgrcv(msgrid,(void *)&some_data,MAX_TEXT,0,0);
        if (some_data.nbCook < MAX_COOK) {
            std::cout << GREEN << "cook = " << some_data.nbCook << RESET << std::endl;
            createProcess = false;
            kitchenID=processVector.at(i);
            break;
        } else {
            msgsnd(processVector.at(i), (void *)&some_data, MAX_TEXT, IPC_NOWAIT);
        }
    }
    if (createProcess) {
        key_t newKey = processVector.size() + 100;
        std::cout << "pass, new key : " << newKey << std::endl;
        ipc(newKey);
        processVector.push_back(msgget(newKey, 0666|IPC_CREAT));
        kitchenID=processVector.back();
    }

    some_data.msg_type=1;
    strcpy(some_data.some_text, line.c_str());
    std::cout << "ID: " << kitchenID << std::endl;
    msgsnd(kitchenID, (void *)&some_data, MAX_TEXT, IPC_NOWAIT);
    return processVector;
}

/*
** EPITECH PROJECT, 2024
** B-CCP-400-PAR-4-1-theplazza-yanis.harkouk
** File description:
** IPC
*/

#ifndef IPC_HPP_
#define IPC_HPP_

#include <vector>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include <queue>
#include <cstring>
#include <thread>

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define RESET "\e[0m"

#define MAX_TEXT 512
#define MAX_COOK 5

struct my_msg {
    long int msg_type;
    int nbCook;
    char some_text[MAX_TEXT];
    std::queue<std::string> msgQueue;
};

class IPC {
    public:
        IPC() {};

        static std::vector<key_t> comm(std::string line, std::vector<key_t> processVector);
    private:
        static void ipc(int processNum);
};

#endif /* !IPC_HPP_ */

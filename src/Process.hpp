/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Process
*/

#ifndef PROCESS_HPP_
#define PROCESS_HPP_

    #include "unistd.h"

    #include <cstring>
    #include <sys/msg.h>
    #include <vector>
    #include <string>
    #include "Arguments.hpp"
    #include "PlazzaMenu.hpp"

struct my_msg {
    int nbCook;
    char msg[100];
};

namespace plazza {
    class Process {
        public:
            Process();
            ~Process();

            pid_t forkFunc();
            void IPC(std::string pizzaInfoString);
            void forkKitchen(int processNum);
            plazza::Arguments _args;
        private:
            void createNewProcess(struct my_msg senderReception, struct my_msg receiverReception, std::string pizzaInfoString);

            int _msgrid;
            plazza::PlazzaMenu _menu;
            std::vector<int> _processVector;
            std::size_t _nbProcess;
    };
}

#endif /* !PROCESS_HPP_ */

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
    char msg[7];
};

namespace plazza {
    class Process {
        public:
            Process();
            ~Process();

            pid_t forkFunc();
            void IPC(std::string pizzaInfoString);
            void forkKitchen(int processNum, int msgrid);
            plazza::Arguments _args;
        private:
            void createNewProcess(bool createProcess, std::string pizzaInfoString);

            int msgrid;
            plazza::PlazzaMenu _menu;
            std::vector<int> _processVector;
            int nbProcess;
    };
}

#endif /* !PROCESS_HPP_ */

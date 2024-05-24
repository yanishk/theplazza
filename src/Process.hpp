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
            class FunctionFailure : public virtual plazza::Error
            {
                public:
                    FunctionFailure(const std::string& function):
                    plazza::Error(function + ": failed") {}
            };
            Process(plazza::Arguments args, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenuVector, std::vector<std::string> plazzaIngredientsVector);
            ~Process() = default;

            void IPC(std::string pizzaInfoString);
            plazza::Arguments _args;
        private:
            void createNewProcess(bool createProcess, std::string pizzaInfoString);
            pid_t forkFunc();
            void forkKitchen(int processNum, int msgrid);

            int _msgrid;
            plazza::PlazzaMenu _menu;
            std::vector<int> _processVector;
            std::size_t _nbProcess;
            std::vector<std::shared_ptr<plazza::Pizza>>  _plazzaMenuVector;
            std::vector<std::string> _plazzaIngredientsVector;
    };
}

#endif /* !PROCESS_HPP_ */

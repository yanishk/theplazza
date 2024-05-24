/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Process
*/

#ifndef PROCESS_HPP_
#define PROCESS_HPP_

    #include <unistd.h>
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

namespace plazza
{
    class Process
    {
        public:
            class FunctionFailure : public virtual plazza::Error
            {
                public:
                    FunctionFailure(const std::string& function):
                    plazza::Error(function + ": failed") {}
            };
            class CFunction {
                public:
                    static std::size_t forkFunc();
                    static void msgrcvFunc(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
                    static void msgsndFunc(int msqid, const void *msgp, size_t msgsz, int msgflg);
                    static void msgctlFunc(int msqid, int cmd, msqid_ds *buf);
                    static int msggetFunc(key_t key, int msgflg);
                    static key_t ftokFunc(const char *pathname, int proj_id);
            };
            Process(plazza::Arguments args, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenuVector, std::vector<std::string> plazzaIngredientsVector);
            ~Process() = default;
            void IPC(std::string &pizzaInfoString);

        private:
            void forkKitchen(int processNum);
            void createNewProcess(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString);
            void getStatus(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString);
            void sendPizza(struct my_msg &senderReception, struct my_msg &receiverReception, std::string &pizzaInfoString);
            int _msgrid;
            std::vector<std::shared_ptr<plazza::Pizza>>  _plazzaMenuVector;
            std::vector<std::string> _plazzaIngredientsVector;
            plazza::Arguments _args;
            std::vector<int> _processVector;
            std::size_t _nbProcess;
    };
}

#endif /* !PROCESS_HPP_ */

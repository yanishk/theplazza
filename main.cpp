#include <iostream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <cstring>
#include <iomanip>
#include <thread>
#include "Arguments.cpp"
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include <queue>


#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define RESET "\e[0m"

#define MAX_TEXT 512
#define MAX_COOK 5

#define MAX_KITCHENS 1024

struct my_msg {
    long int msg_type;
    int nbCook;
    char some_text[MAX_TEXT];
    std::queue<std::string> msgQueue;
};

int getHelp(int ac, const char **av)
{
    if (ac == 2 && av[1] == "--help") {
        std::cerr << "Usage: ./plazza <multiplier> <nbr_cooks> <milliseconds>" << std::endl;
        return 0;
    }
    if (ac != 4) {
        std::cerr << "Usage: ./plazza <multiplier> <nbr_cooks> <milliseconds>" << std::endl;
        return 84;
    }
    return 0;
}

std::string fillMyMessage(std::string msg)
{
    for (int i = msg.size(); i < MAX_TEXT; i++) {
        msg.append("\0");
    }
    return msg;
}

void ipc(std::vector<int[2]> &pipefdsVector, int fdID)
{
    pid_t pid = fork();
    int nbr = 0;
    int nbr_ko = 0;
    if (pid != 0) {
        while (1) {
            char readmessage[MAX_TEXT];
            std::string msg;
            close(pipefdsVector.at(0)[0]);
            close(pipefdsVector.at(fdID)[1]);
            read(pipefdsVector.at(fdID)[0], readmessage, MAX_TEXT);
            // int choice = std::atoi(readmessage);
            printf("%s %ld\n", readmessage, strlen(readmessage));
            if (std::strcmp(readmessage, "ok") == 0)
                nbr++;
            else
                nbr_ko++;
            
            if (nbr_ko >= 15 && fdID == 1) {
                nbr = 0;
                nbr_ko = 0;
            }
            write(pipefdsVector.at(0)[1], fillMyMessage(std::to_string(nbr)).c_str(), MAX_TEXT);
        }
    }
}

int main(int ac, const char **av) {
    plazza::Arguments args;

    if (getHelp(ac, av) == 84)
        return 84;
    if (args.retrieveArgs(av) == 84)
        return 84;

    std::string line;

    std::vector<int[2]> pipefdsVector(MAX_KITCHENS);
    for (int i = 0; i < MAX_KITCHENS; i++) {
        pipe(pipefdsVector[i]);
    }
    ipc(pipefdsVector, 1);
    int fdID = 1;

    while (std::getline(std::cin, line)) {
        bool is_all_full = false;
        int freeID = 0;
        char finalmessage[MAX_TEXT];
        for (int i = 0; i < fdID; i++) {
            char readmessage[MAX_TEXT];
            write(pipefdsVector.at(i + 1)[1], fillMyMessage("ko").c_str(), MAX_TEXT);
            read(pipefdsVector.at(0)[0], readmessage, MAX_TEXT);
            int nb = std::atoi(readmessage);
            if (nb >= MAX_COOK) {
                is_all_full = true;
            } else {
                freeID = i + 1;
                is_all_full = false;
                break;
            }
        }
        if (is_all_full) {
            fdID++;
            ipc(pipefdsVector, fdID);
            freeID = fdID;
        }
        write(pipefdsVector.at(freeID)[1], fillMyMessage("ok").c_str(), MAX_TEXT);
        read(pipefdsVector.at(0)[0], finalmessage, MAX_TEXT);
        printf("In Child: Reading from pipe %d - Message is %s\n",freeID, finalmessage);

        if (line == "exit")
            break;
    }
    return 0;
}

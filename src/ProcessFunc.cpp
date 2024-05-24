/*
** EPITECH PROJECT, 2024
** reclone
** File description:
** ProcessFunc
*/

#include "Process.hpp"

std::size_t plazza::Process::CFunction::forkFunc()
{
    std::size_t pid = fork();
    if (pid == -1)
        throw plazza::Process::FunctionFailure("fork");
    return pid;
}

void plazza::Process::CFunction::msgrcvFunc(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
    if (msgrcv(msqid, msgp, msgsz, msgtyp, msgflg) == -1)
        throw plazza::Process::FunctionFailure("msgrcv");
}

void plazza::Process::CFunction::msgsndFunc(int msqid, const void *msgp, size_t msgsz, int msgflg)
{
    if (msgsnd(msqid, msgp, msgsz, msgflg) == -1)
        throw plazza::Process::FunctionFailure("msgrcv");
}

void plazza::Process::CFunction::msgctlFunc(int msqid, int cmd, msqid_ds *buf)
{
    if (msgctl(msqid, cmd, buf) == -1)
        throw plazza::Process::FunctionFailure("msgctl");
}

int plazza::Process::CFunction::msggetFunc(key_t key, int msgflg)
{
    int returnValue = msgget(key, msgflg);
    if (returnValue == -1)
        throw plazza::Process::FunctionFailure("msgget");
    return returnValue;
}

key_t plazza::Process::CFunction::ftokFunc(const char *pathname, int proj_id)
{
    int returnValue = ftok(pathname, proj_id);
    if (returnValue == -1)
        throw plazza::Process::FunctionFailure("ftok");
    return returnValue;
}

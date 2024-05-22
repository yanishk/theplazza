/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Process
*/

#include "Process.hpp"

plazza::Process::Process()
{
}

plazza::Process::~Process()
{
}

pid_t plazza::Process::forkFunc() {
    return fork();
}

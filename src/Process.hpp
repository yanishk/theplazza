/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Process
*/

#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include "unistd.h"

namespace plazza {
    class Process {
        public:
            Process();
            ~Process();

            pid_t forkFunc();

        protected:
        private:
    };
}

#endif /* !PROCESS_HPP_ */

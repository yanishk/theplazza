/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Waiter
*/

#ifndef WAITER_HPP_
#define WAITER_HPP_

    #include <vector>
    #include <iostream>
    #include "Error.hpp"
    #include "Stock.hpp"
    #include "ThreadPool.hpp"

namespace plazza
{
    class Waiter
    {
        public:
            class ParsingCommandFailed : public plazza::Error
            {
                public:
                    ParsingCommandFailed(const std::string& errorMessage):
                    plazza::Error("Parsing command: " + errorMessage) {}
            };

            Waiter();
            ~Waiter() = default;
            void placeOrders(plazza::Stock stock, int nbrCooks);

        private:
            void parseMultipleCommands(std::string &line);
            void fillCommands();
            std::vector<std::string> parseCommand(std::string &line);
            void parseWord(std::string &line, std::vector<std::string> &commandInfos, const std::size_t &wordIndex);
            bool isDelim(std::string &line);
            void checkCommandEnd(const std::string &line);
            void changeCommandsCase();
            void checkCommands();
            void printTmpCommands();
            void printCommands();
            void resetCommandList();
            void lowerToUpperCase(std::string &word);
            std::vector<std::string> _pizzaTypeVector;
            std::vector<std::string> _pizzaSizeVector;
            std::vector<std::vector<std::string>> _tmpCommandsVector;
            std::vector<std::pair<std::string, std::size_t>> _commandsVector;
            const std::size_t _commandArguments = 3;
    };
}

#endif

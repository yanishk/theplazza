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
    #include <tuple>
    #include "Error.hpp"
    #include "Stock.hpp"
    #include "ThreadPool.hpp"
    #include "Arguments.hpp"
    #include "PlazzaMenu.hpp"

namespace plazza
{
    class Waiter
    {
        public:
            class ParsingCommandFailed : public virtual plazza::Error
            {
                public:
                    ParsingCommandFailed(const std::string& errorMessage):
                    plazza::Error("Parsing command: " + errorMessage) {}
            };

            Waiter();
            ~Waiter() = default;
            void placeOrders(plazza::Arguments args);
            std::vector<std::tuple<std::string, std::string, std::size_t>> getCommands();

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
            void upperToLowerCase(std::string &word);
            void lowertoUpperCase(std::string &word);
            plazza::PlazzaMenu _menu;
            std::vector<std::string> _plazzaIngredientsList;
            std::vector<std::string> _pizzaTypeVector;
            std::vector<std::string> _pizzaSizeVector;
            std::vector<std::vector<std::string>> _tmpCommandsVector;
            std::vector<std::tuple<std::string, std::string, std::size_t>> _commandsVector;
            const std::size_t _commandArguments = 3;
    };
}

#endif

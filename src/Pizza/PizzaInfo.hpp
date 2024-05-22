/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** PizzaInfo
*/

#ifndef PIZZAINFO_HPP_
#define PIZZAINFO_HPP_

#include "Error.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace plazza
{
    enum PizzaType
    {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };

    enum PizzaSize
    {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    class PizzaInfo
    {
        public:
            class PizzaInfoError : public virtual plazza::Error
            {
                public:
                    PizzaInfoError(const std::string& message):
                    plazza::Error("PizzaInfo: " + message) {}
            };

            PizzaInfo(plazza::PizzaType type, plazza::PizzaSize size);
            PizzaInfo(std::string &stringType, std::string &stringSize);
            std::string getPizzaTypeString();
            std::string getPizzaSizeString();
            plazza::PizzaType getPizzaType();
            plazza::PizzaSize getPizzaSize();
            void setPizzaType(plazza::PizzaType type);
            void setPizzaSize(plazza::PizzaSize size);

        private:
            void findStringType(std::string &stringType);
            void findStringSize(std::string &stringType);
            void initializePizzaInfoVectors();
            void initializeSizeVector();
            void initializeTypeVector();
            plazza::PizzaType _type;
            plazza::PizzaSize _size;
            std::vector<std::pair<std::string, plazza::PizzaType>> _typeVector;
            std::vector<std::pair<std::string, plazza::PizzaSize>> _sizeVector;
    };
}

void operator<<(std::string &objInfos, plazza::PizzaInfo &pizza);
void operator>>(const std::string &objInfos, plazza::PizzaInfo &pizza);

#endif

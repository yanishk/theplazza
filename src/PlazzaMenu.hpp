/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** PlazzaMenu
*/

#ifndef PLAZZAMENU_HPP_
#define PLAZZAMENU_HPP_

    #include "Pizza.hpp"
    #include "Error.hpp"
    #include <vector>
    #include <string>
    #include <memory>
    #include <fstream>
    #include <algorithm>

namespace plazza
{
    class PlazzaMenu
    {
        public:
            class ParsingPlazzaMenuFailed : public plazza::Error
            {
                public:
                    ParsingPlazzaMenuFailed(const std::string& errorMessage):
                    plazza::Error("Parsing plazza menu: " + errorMessage) {}
            };

            PlazzaMenu();
            std::vector<std::shared_ptr<plazza::Pizza>> getMenu();
            std::vector<std::string> getIngredients();
            std::vector<std::string> getPizzaSizes();

        private:
            void parsePizzaIngredients(std::string &line);
            void parsePizzaIngredientsLine(std::string &line);
            void parsePizzaCookingTimeLine(std::string &line);
            void parsePizzaNameLine(std::string &line);
            void parseIngredientsList(std::string &line);
            void parseIngredientsListLine(std::string &line);
            void parsePizzaSizesList(std::string &line);
            void parsePizzaSizesListLine(std::string &line);
            void parsePlazzaMenuFile(std::string fileName);
            std::vector<std::shared_ptr<plazza::Pizza>> _plazzaMenuVector;
            std::vector<std::string> _plazzaIngredientsVector;
            std::vector<std::string> _plazzaPizzaSizesVector;
    };
}

#endif /* !PLAZZAMENU_HPP_ */

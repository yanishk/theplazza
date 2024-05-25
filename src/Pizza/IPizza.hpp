/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** IPizza
*/

#ifndef IPIZZA_HPP_
#define IPIZZA_HPP_

#include "Arguments.hpp"
#include "Stock.hpp"

namespace plazza
{
    class IPizza
    {
        public:
            virtual ~IPizza() = default;

            virtual void setPizzaName(std::string &pizzaName) = 0;
            virtual std::string getPizzaName() = 0;
            virtual void setCookingTime(std::size_t cookingTime) = 0;
            virtual std::size_t getCookingTime() = 0;
            virtual std::size_t getIngredientNumber() = 0;
            virtual void pushIngredient(std::string &ingredientName) = 0;
            virtual void cookPizza(const std::string pizzaSize, float multiplier, std::shared_ptr<plazza::Stock> stock,
                std::vector<std::pair<std::string, std::string>> &pizzaInCharge, std::size_t nbProcess, std::chrono::_V2::steady_clock::time_point &clockT0) = 0;
    };
}

#endif /* !IPIZZA_HPP_ */

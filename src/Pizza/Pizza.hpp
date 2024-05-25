/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Pizza
*/

#ifndef PIZZA_HPP_
#define PIZZA_HPP_

#include "IPizza.hpp"
#include <vector>
#include <memory>

namespace plazza
{
    class Pizza : virtual plazza::IPizza
    {
        public:
            virtual ~Pizza() = default;
            virtual void setPizzaName(std::string &pizzaName) override;
            virtual std::string getPizzaName() override;
            virtual void setCookingTime(std::size_t cookingTime) override;
            virtual std::size_t getCookingTime() override;
            virtual std::size_t getIngredientNumber() override;
            virtual void pushIngredient(std::string &ingredientName) override;
            virtual void cookPizza(const std::string pizzaSize, float multiplier, std::shared_ptr<plazza::Stock> stock,
                std::vector<std::pair<std::string, std::string>> &pizzaInCharge, std::size_t nbProcess, std::chrono::_V2::steady_clock::time_point &clockT0) override;

        private:
            std::vector<std::string> _margIngredients;
            std::string _pizzaName;
            std::size_t _cookingTime;
            std::mutex _mutexPizza;
    };
}

#endif /* !IPIZZA_HPP_ */

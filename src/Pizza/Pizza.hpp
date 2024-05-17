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
            virtual void cookPizza(float multiplier, plazza::Stock &stock) override;
            std::vector<std::string> _margIngredients;

        private:
            std::string _pizzaName;
            std::size_t _cookingTime;
    };
}

#endif /* !IPIZZA_HPP_ */

/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Margarita
*/

#ifndef MARGARITA_HPP_
#define MARGARITA_HPP_

#include <thread>
#include <chrono>
#include <vector>
#include "IPizza.hpp"

namespace plazza {
    class Margarita : public IPizza {
        public:
            Margarita();
            ~Margarita();

            void cookPizza(float multiplier, plazza::Stock *stock) override;

        protected:
        private:
            std::vector<std::string> _MargIngredients;
    };
}

#endif /* !MARGARITA_HPP_ */

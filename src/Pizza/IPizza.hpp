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

namespace plazza {
    class IPizza {
        public:
            virtual ~IPizza() = default;

            virtual void cookPizza(float multiplier, plazza::Stock *stock) = 0;

        protected:
        private:
    };
}

#endif /* !IPIZZA_HPP_ */

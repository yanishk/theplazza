/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Stock
*/

#ifndef STOCK_HPP_
#define STOCK_HPP_

#include <iostream>
#include <map>
#include <chrono>
#include <thread>

namespace plazza {
    class Stock {
        public:
            Stock();
            ~Stock();

            static void refillStock(plazza::Stock stock, int time);
            void takeFromStock(std::string);

        protected:
            std::map<std::string, std::size_t> _stockIngredients;
        private:
    };
}
#endif /* !STOCK_HPP_ */

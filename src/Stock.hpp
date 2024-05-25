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
#include <vector>
#include <memory>
#include <condition_variable>

namespace plazza
{
    class Stock
    {
        public:
            Stock(std::vector<std::string> ingredientsList);
            ~Stock() = default;

            static void refillStock(std::shared_ptr<plazza::Stock> stock, int time);
            void takeFromStock(std::string);
            std::map<std::string, std::size_t> getStock();

        protected:
        private:
            std::map<std::string, std::size_t> _stockIngredients;
            static inline std::condition_variable _stockCondVar;
            std::mutex _stockMutex;
    };
}
#endif /* !STOCK_HPP_ */

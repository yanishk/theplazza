/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Kitchen
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include "ThreadPool.hpp"
#include "Stock.hpp"
#include "PlazzaMenu.hpp"
#include "Error.hpp"

namespace plazza
{
    class Kitchen
    {
        public:
            class PizzaUnrecognized : public virtual plazza::Error
            {
                public:
                    PizzaUnrecognized():
                    plazza::Error("Kitchen: Chosen pizza size doesn't exist") {}
            };

            Kitchen(int nbrCooks, int milliseconds, float multiplier, std::vector<std::shared_ptr<plazza::Pizza>> plazzaMenu, std::vector<std::string> ingredientsList);
            ~Kitchen();
            bool needToCloseKitchen();
            void makePizza(std::string &pizzaName, std::string &pizzaSize);
            std::shared_ptr<plazza::Stock> getStockObject();
            int getPizzasInWork();
            int getAvailableCooks();
            int getSpaceLeft();
            void makeStatus(std::size_t nbProcess);

        private:
            float _multiplier;
            plazza::ThreadPool _threadPool;
            std::shared_ptr<plazza::Stock> _stock;
            std::unordered_map<std::string, std::shared_ptr<plazza::Pizza>> _availablePizzas;
            std::thread _stockThread;
            int _millisecond;
            std::vector<std::pair<std::string, std::string>> _pizzaInCharge;
            std::chrono::_V2::steady_clock::time_point _clockT0;
            std::chrono::_V2::steady_clock::time_point _clockT1;
    };
}

#endif
/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Arguments
*/

#ifndef ARGUMENTS_HPP_
#define ARGUMENTS_HPP_

    #include "Error.hpp"
    #include <string>

namespace plazza
{
    class Arguments
    {
        public:
            class ExpectedPositiveValues : public virtual plazza::Error
            {
                public:
                    ExpectedPositiveValues():
                    plazza::Error("Arguments: expected positive values only") {}
            };
            class FunctionFailure : public virtual plazza::Error
            {
                public:
                    FunctionFailure(const std::string& function):
                    plazza::Error(function + ": failed") {}
            };

            Arguments() = default;
            ~Arguments() = default;

            void retrieveArgs(const char **av);
            void checkArgValidity() const;

            int getNbrCooks() const noexcept;
            int getMilliseconds() const noexcept;
            float getMultiplier() const noexcept;

        private:
            float _multiplier;
            int _nbrCooks;
            int _milliseconds;
    };
}

#endif /* !ARGUMENTS_HPP_ */

/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** Arguments
*/

#ifndef ARGUMENTS_HPP_
#define ARGUMENTS_HPP_

namespace plazza
{
    class Arguments
    {
        public:
            Arguments() = default;
            ~Arguments() = default;

            int retrieveArgs(const char **av);
            int checkArgValidity() const;

            int getNbrCooks() const noexcept;
            int getMilliseconds() const noexcept;
            float getMultiplier() const noexcept;

        protected:
            float _multiplier;
            int _nbrCooks;
            int _milliseconds;
        private:
    };
}

#endif /* !ARGUMENTS_HPP_ */

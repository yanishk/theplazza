/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** ThreadPool
*/

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <chrono>

namespace plazza {
    class ThreadPool {
        public:
            ThreadPool(std::size_t number);
            ~ThreadPool();

            void putInQueue(std::function<void()> work);
            bool getDone();
            int getWork();
            int getAvailableCooks();
            int getSpaceLeft();

        protected:
        private:
            std::queue<std::function<void()>> _work;
            std::vector<std::thread> _threads;
            std::size_t _nbThreads;
            std::mutex _mutexPool;
            std::condition_variable _condVar;
            bool _done;
            std::chrono::_V2::steady_clock::time_point _clock;
            std::size_t _countPizza;
            std::size_t _pizzasIn;

    };
}

#endif /* !THREADPOOL_HPP_ */

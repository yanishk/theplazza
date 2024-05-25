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

namespace plazza
{
    class ThreadPool
    {
        public:
            ThreadPool(std::size_t number);
            ~ThreadPool();

            void threadRoutine();
            void putInQueue(std::function<void()> work);
            bool getDone();
            int getWork();
            int getAvailableThreads();
            int getSpaceLeft();

        private:
            std::queue<std::function<void()>> _work;
            std::vector<std::thread> _threads;
            std::mutex _mutexPool;
            std::condition_variable _condVar;
            std::size_t _nbThreads;
            bool _done;
            std::size_t _countThreadWorking;
            std::size_t _countFunctionsInQueue;

    };
}

#endif /* !THREADPOOL_HPP_ */

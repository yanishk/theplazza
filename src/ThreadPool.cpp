/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** ThreadPool
*/

#include "ThreadPool.hpp"

plazza::ThreadPool::ThreadPool(std::size_t number) : _nbThreads(number)
{
    for (std::size_t i = 0; i != _nbThreads; i++) {
        _threads.emplace_back(
            [this] {
                while (1) {
                    std::function<void()> work;
                    {
                        std::unique_lock<std::mutex> lock(_mutexPool);
                        _condVar.wait(lock, [this] { return !_work.empty();});
                        if (_work.empty())
                            return;
                        work = _work.front();
                        _work.pop();
                    }
                    work();
                }
            }
        );
    }
}

plazza::ThreadPool::~ThreadPool()
{
    _condVar.notify_all();
    for (auto &it : _threads)
        it.join();
}

void plazza::ThreadPool::putInQueue(std::function<void()> work)
{
    {
        std::unique_lock<std::mutex> lock(_mutexPool);
        _work.push(work);
    }
    _condVar.notify_one();
}

/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** ThreadPool
*/

#include "ThreadPool.hpp"

plazza::ThreadPool::ThreadPool(std::size_t number) : _nbThreads(number), _done(false),
    _countThreadWorking(0), _countFunctionsInQueue(0)
{
    for (std::size_t i = 0; i != _nbThreads; i++) {
        _threads.emplace_back(
            [this] {
                threadRoutine();
            }
        );
    }
}

plazza::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(_mutexPool);
        _done = true;
    }
    _condVar.notify_all();
    for (auto &it : _threads)
        it.join();
}

void plazza::ThreadPool::threadRoutine()
{
    while (1) {
        std::function<void()> work;
        {
            std::unique_lock<std::mutex> lock(_mutexPool);
            _condVar.wait(lock, [this] { return !_work.empty() || _done;});
            if (_work.empty() && _done)
                return;
            work = _work.front();
            _work.pop();
            _countThreadWorking++;
        }
        work();
        _countThreadWorking--;
        _countFunctionsInQueue--;
    }
}

void plazza::ThreadPool::putInQueue(std::function<void()> work)
{
    {
        std::unique_lock<std::mutex> lock(_mutexPool);
        _work.push(work);
        _countFunctionsInQueue++;
    }
    _condVar.notify_one();
}

bool plazza::ThreadPool::getDone()
{
    return _done;
}

int plazza::ThreadPool::getWork()
{
    return _countThreadWorking;
}

int plazza::ThreadPool::getAvailableThreads()
{
    int availThreads = _nbThreads - _countThreadWorking;

    if (availThreads < 0)
        return 0;
    return availThreads;
}

int plazza::ThreadPool::getSpaceLeft()
{
    return (_nbThreads * 2) - _countFunctionsInQueue;
}

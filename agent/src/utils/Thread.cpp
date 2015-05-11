#include <thread>

#include "Thread.hpp"

bool tin::utils::Thread::isRunning()
{
    return this->running;
}

std::thread tin::utils::Thread::createThread()
{
    return std::thread([this]() {
        while (this->isRunning())
        {
            this->threadTask();
        }
    });
}

void tin::utils::Thread::terminate()
{
    this->running = false;
}

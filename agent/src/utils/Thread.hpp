#ifndef TIN_UTILS_THREAD_HPP
#define TIN_UTILS_THREAD_HPP

#include <thread>

namespace tin { namespace utils
{
    class Thread
    {
    private:
        bool running = true;

        bool isRunning();

    public:
        virtual ~Thread() {};
        virtual std::thread createThread();
        virtual void threadTask() = 0;
        virtual void terminate();
    };
}}

#endif	/* TIN_UTILS_THREAD_HPP */


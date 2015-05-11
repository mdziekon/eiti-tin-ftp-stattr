#ifndef TIN_UTILS_QUEUETHREAD_HPP
#define	TIN_UTILS_QUEUETHREAD_HPP

#include "Thread.hpp"
#include <concurrent-queue/Queue.hpp>
#include <memory>

namespace tin { namespace utils
{
    template<typename EventClass, typename VisitorClass>
    class QueueThread: public tin::utils::Thread
    {
    protected:
        Queue<std::shared_ptr<EventClass>> &incomingQueue;
        VisitorClass visitor;

        QueueThread(Queue<std::shared_ptr<EventClass>> &queue, VisitorClass &visitor);
        QueueThread(Queue<std::shared_ptr<EventClass>> &queue, VisitorClass &&visitor);
    public:
        void threadTask();
    };
}}

#include "QueueThread.impl.hpp"

#endif	/* TIN_UTILS_QUEUETHREAD_HPP */


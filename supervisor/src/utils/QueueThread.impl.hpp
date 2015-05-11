#ifndef TIN_UTILS_QUEUETHREAD_IMPL_HPP
#define	TIN_UTILS_QUEUETHREAD_IMPL_HPP

template<typename EventClass, typename VisitorClass>
tin::utils::QueueThread<EventClass, VisitorClass>::QueueThread(
    Queue<std::shared_ptr<EventClass>> &queue,
    VisitorClass &visitor
):
incomingQueue(queue),
visitor(visitor)
{}

template<typename EventClass, typename VisitorClass>
tin::utils::QueueThread<EventClass, VisitorClass>::QueueThread(
    Queue<std::shared_ptr<EventClass>> &queue,
    VisitorClass &&visitor
):
incomingQueue(queue),
visitor(visitor)
{}

template<typename EventClass, typename VisitorClass>
void tin::utils::QueueThread<EventClass, VisitorClass>::threadTask()
{
    this->incomingQueue.pop()->accept(this->visitor);
}

#endif	/* TIN_UTILS_QUEUETHREAD_IMPL_HPP */


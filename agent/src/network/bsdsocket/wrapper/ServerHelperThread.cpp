#include "ServerHelperThread.hpp"

#include "Event.hpp"
#include "Server.hpp"

using tin::network::bsdsocket::wrapper::ServerHelperThread;

ServerHelperThread::ServerHelperThread(
    tin::network::bsdsocket::wrapper::Server& server,
    tin::network::bsdsocket::wrapper::ServerQueue& incomingQueue
):
server(server),
incomingQueue(incomingQueue)
{}

void ServerHelperThread::threadTask()
{
    tin::network::bsdsocket::wrapper::EventPtr e = this->incomingQueue.pop();
    (*e).accept(server.visitor);
}


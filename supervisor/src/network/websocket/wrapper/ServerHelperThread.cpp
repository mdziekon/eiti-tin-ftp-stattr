#include "ServerHelperThread.hpp"

#include "Event.hpp"
#include "Server.hpp"

using tin::network::websocket::wrapper::ServerHelperThread;

ServerHelperThread::ServerHelperThread(
    tin::network::websocket::wrapper::Server& server,
    tin::network::websocket::wrapper::ServerQueue& incomingQueue
):
server(server),
incomingQueue(incomingQueue)
{}

void ServerHelperThread::threadTask()
{
    tin::network::websocket::wrapper::EventPtr e = this->incomingQueue.pop();
    (*e).accept(server.visitor);
}


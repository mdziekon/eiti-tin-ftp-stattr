#include "ClientHelperThread.hpp"

#include "Event.hpp"
#include "Client.hpp"

using tin::network::bsdsocket::wrapper::ClientHelperThread;

ClientHelperThread::ClientHelperThread(
    tin::network::bsdsocket::wrapper::Client& client,
    tin::network::bsdsocket::wrapper::ClientQueue& incomingQueue
):
client(client),
incomingQueue(incomingQueue)
{}

void ClientHelperThread::threadTask()
{
    tin::network::bsdsocket::wrapper::EventPtr e = this->incomingQueue.pop();
    (*e).accept(client.visitor);
}


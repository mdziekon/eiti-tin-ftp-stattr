#include "Server.hpp"

#include <functional>
#include <utility>
#include <exception>

#include "events/IncomingMessage.hpp"
#include "events/OutcomingMessage.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::wrapper::Server;
namespace events = tin::network::bsdsocket::wrapper::events;

Server::Server(const unsigned int& portNo):
portNo(portNo),
receiverHelperThread(*this, this->receiverQueue),
transmitterHelperThread(*this, this->transmitterQueue),
visitor(*this)
{
    // Initialize stuff
}

Server::~Server()
{
    this->terminate();
}

void Server::run()
{
    this->listenerThread = std::thread(
        &Server::listenerLoop,
        std::ref(*this)
    );
    this->receiverThread = this->receiverHelperThread.createThread();
    this->transmitterThread = this->transmitterHelperThread.createThread();

    this->listenerThread.detach();
    this->receiverThread.detach();
    this->transmitterThread.detach();
}

void Server::terminate()
{
    this->receiverHelperThread.terminate();
    this->transmitterHelperThread.terminate();
    this->receiverQueue.push(
        EventPtr(new events::Terminate())
    );
    this->transmitterQueue.push(
        EventPtr(new events::Terminate())
    );
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const std::string&)>& handler
)
{
    return this->messageReceivedHandlers.insert(handler);
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const std::string&)>&& handler
)
{
    return this->messageReceivedHandlers.insert(
        std::forward<std::function<void(const std::string&)>>(handler)
    );
}

void Server::sendResponse(
    const std::string& message
)
{
    this->transmitterQueue.push(
        EventPtr(new events::OutcomingMessage(message))
    );
}

void Server::terminateConnection()
{

}


void Server::runMessageReceivedHandlers(
    const std::string& message
)
{
    for (auto iter: this->messageReceivedHandlers)
    {
        iter.second(message);
    }
}


void Server::listenerLoop()
{}

void Server::onMessageReceive(const std::string& message)
{
    this->receiverQueue.push(
        EventPtr(new events::IncomingMessage(message))
    );
}

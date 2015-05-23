#include "Manager.hpp"

#include <iostream>

#include "events/NewServerConnection.hpp"
#include "events/ServerConnectionClosed.hpp"
#include "events/MessageReceived.hpp"

using tin::network::websocket::Manager;
namespace events = tin::network::websocket::events;

Manager::Manager(
    tin::network::websocket::ManagerQueue& incomingQueue,
    tin::controllers::main::ControllerQueue& controllerQueue,
    const unsigned int& serverPortNo
):
QueueThread(incomingQueue, ManagerVisitor(*this)),
server(serverPortNo),
controllerQueue(controllerQueue)
{
    this->server.run();

    this->server.attachConnectionOpenedHandler([this](const unsigned int& idx) {
        this->incomingQueue.push(
            EventPtr(new events::NewServerConnection(idx))
        );
    });
    this->server.attachConnectionClosedHandler([this](const unsigned int& idx) {
        this->incomingQueue.push(
            EventPtr(new events::ServerConnectionClosed(idx))
        );
    });
    this->server.attachMessageReceivedHandler([this](const unsigned int& idx, const std::string& msg) {
        this->incomingQueue.push(
            EventPtr(new events::MessageReceived(idx, msg))
        );
    });
}

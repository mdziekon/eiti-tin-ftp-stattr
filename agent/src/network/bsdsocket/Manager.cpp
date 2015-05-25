#include "Manager.hpp"

#include <iostream>

#include "events/MessageReceived.hpp"
#include "events/ResponseRequest.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::Manager;
namespace events = tin::network::bsdsocket::events;

Manager::Manager(
    tin::network::bsdsocket::ManagerQueue& incomingQueue,
    tin::controllers::main::ControllerQueue& controllerQueue,
    const unsigned int& serverPortNo
):
QueueThread(incomingQueue, ManagerVisitor(*this)),
server(serverPortNo),
controllerQueue(controllerQueue)
{
    this->server.run();

    this->server.attachMessageReceivedHandler([this](const std::string& msg) {
        this->incomingQueue.push(
            EventPtr(new events::MessageReceived(msg))
        );
    });

}

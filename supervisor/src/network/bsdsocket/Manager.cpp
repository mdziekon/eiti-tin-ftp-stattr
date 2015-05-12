#include "Manager.hpp"

#include <iostream>

#include "events/MessageRequest.hpp"
#include "events/ResponseReceived.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::Manager;
namespace events = tin::network::bsdsocket::events;

Manager::Manager(
    tin::network::bsdsocket::ManagerQueue& incomingQueue,
    tin::controllers::main::ControllerQueue& controllerQueue
):
QueueThread(incomingQueue, ManagerVisitor(*this)),
controllerQueue(controllerQueue)
{
    this->client.attachResponseReceivedHandler([this](const std::string& ip, const unsigned int& port, const std::string& message) {
        this->incomingQueue.push(
            EventPtr(new events::ResponseReceived(ip, port, message))
        );
    });
}

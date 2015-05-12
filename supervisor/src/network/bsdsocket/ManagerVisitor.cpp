#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/Terminate.hpp"
#include "events/ResponseReceived.hpp"
#include "events/MessageRequest.hpp"

using tin::network::bsdsocket::ManagerVisitor;
namespace events = tin::network::bsdsocket::events;

#include <iostream>

void ManagerVisitor::visit(events::Terminate& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::ResponseReceived& evt)
{
    std::cout << "[Supervisor] Received response: " << evt.message << std::endl;
    // std::cout << "[Supervisor] Sending message: " << "test" << std::endl;

    // this->manager.client.sendMessage("test");

    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::MessageRequest& evt)
{
    std::cout << "[Supervisor] Sending message: " << evt.message << std::endl;
    std::cout << "             Destination: " << evt.ip << ":" << evt.port << std::endl;

    this->manager.client.sendMessage(evt.ip, evt.port, evt.message, evt.waitForResponse);
}

ManagerVisitor::ManagerVisitor(tin::network::bsdsocket::Manager& manager):
manager(manager)
{}

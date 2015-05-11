#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/Terminate.hpp"
#include "events/MessageReceived.hpp"
#include "events/ResponseRequest.hpp"

using tin::network::bsdsocket::ManagerVisitor;
namespace events = tin::network::bsdsocket::events;

#include <iostream>

void ManagerVisitor::visit(events::Terminate& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::MessageReceived& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::ResponseRequest& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

ManagerVisitor::ManagerVisitor(tin::network::bsdsocket::Manager& manager):
manager(manager)
{}

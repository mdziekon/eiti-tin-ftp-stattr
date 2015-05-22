#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "../../utils/JSON.hpp"

#include "events/Terminate.hpp"
#include "events/MessageReceived.hpp"
#include "events/ResponseRequest.hpp"
#include "events/ConnectionTerminationRequest.hpp"

#include "../../controllers/main/events/CmdReceived.hpp"

using tin::network::bsdsocket::ManagerVisitor;
namespace events = tin::network::bsdsocket::events;
namespace mainCtrlEvents = tin::controllers::main::events;

#include <iostream>

void ManagerVisitor::visit(events::Terminate& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::MessageReceived& evt)
{
    std::cout << "[Agent] Received message: " << evt.message << std::endl;

    this->manager.controllerQueue.push(
        tin::controllers::main::EventPtr(
            new mainCtrlEvents::CmdReceived(tin::utils::json::makeSharedInstance(evt.message))
        )
    );
}

void ManagerVisitor::visit(events::ResponseRequest& evt)
{
    std::cout << "[Agent] Sending response: " << evt.jsonPtr->dump() << std::endl;

    this->manager.server.sendResponse(evt.jsonPtr->dump());
}

void ManagerVisitor::visit(events::ConnectionTerminationRequest& evt)
{
    std::cout << "[Agent] Terminating connection" << std::endl;

    this->manager.server.terminateConnection();
}

ManagerVisitor::ManagerVisitor(tin::network::bsdsocket::Manager& manager):
manager(manager)
{}

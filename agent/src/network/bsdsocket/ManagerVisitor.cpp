#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include <json/src/json.hpp>

#include "events/Terminate.hpp"
#include "events/MessageReceived.hpp"
#include "events/ResponseRequest.hpp"

#include "../../controllers/main/events/CmdReceived.hpp"

using json = nlohmann::json;
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
            new mainCtrlEvents::CmdReceived(std::shared_ptr<json>(new json(evt.message)))
        )
    );
}

void ManagerVisitor::visit(events::ResponseRequest& evt)
{
    std::cout << "[Agent] Sending response: " << evt.jsonPtr->dump() << std::endl;

    this->manager.server.sendResponse(evt.jsonPtr->dump());
}

ManagerVisitor::ManagerVisitor(tin::network::bsdsocket::Manager& manager):
manager(manager)
{}

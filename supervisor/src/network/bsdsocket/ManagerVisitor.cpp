#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include <json/src/json.hpp>

#include "events/Terminate.hpp"
#include "events/ResponseReceived.hpp"
#include "events/MessageRequest.hpp"

#include "../../controllers/main/events/CmdResponseReceived.hpp"

using json = nlohmann::json;
using tin::network::bsdsocket::ManagerVisitor;
namespace events = tin::network::bsdsocket::events;
namespace mainCtrlEvents = tin::controllers::main::events;

#include <iostream>

void ManagerVisitor::visit(events::Terminate& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::ResponseReceived& evt)
{
    this->manager.controllerQueue.push(
        tin::controllers::main::EventPtr(
            new mainCtrlEvents::CmdResponseReceived(
                evt.ip,
                evt.port,
                std::shared_ptr<json>(new json(json::parse(evt.message)))
            )
        )
    );
}

void ManagerVisitor::visit(events::MessageRequest& evt)
{
    std::cout << "[Supervisor] [BSDManager] Sending message: " << (evt.jsonPtr->dump()) << std::endl;
    std::cout << "                          Destination: " << evt.ip << ":" << evt.port << std::endl;

    this->manager.client.sendMessage(evt.ip, evt.port, evt.jsonPtr->dump(), evt.waitForResponse);
}

ManagerVisitor::ManagerVisitor(tin::network::bsdsocket::Manager& manager):
manager(manager)
{}

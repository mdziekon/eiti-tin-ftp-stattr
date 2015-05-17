#include "MainVisitor.hpp"

#include <iostream>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdReceived.hpp"

#include "../../network/bsdsocket/events/ResponseRequest.hpp"

namespace events = tin::controllers::main::events;
namespace bsdsocketEvents = tin::network::bsdsocket::events;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &event)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdReceived &event)
{
    this->controller.networkManagerQueue.push(
        tin::network::bsdsocket::EventPtr(
            new bsdsocketEvents::ResponseRequest(
                tin::utils::json::makeSharedInstance("{ \"testMessage\": \"testResponse\" }")
            )
        )
    );
}

void tin::controllers::main::MainVisitor::visit(tin::controllers::main::events::PacketReceived& event)
{

}


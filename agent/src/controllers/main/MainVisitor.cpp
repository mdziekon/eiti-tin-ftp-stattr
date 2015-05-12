#include "MainVisitor.hpp"

#include <iostream>

#include "MainModule.hpp"

#include "events/Terminate.hpp"

#include "../../network/bsdsocket/typedefs.hpp"
#include "../../network/bsdsocket/events/MessageReceived.hpp"
#include "../../network/bsdsocket/events/ResponseRequest.hpp"

namespace events = tin::controllers::main::events;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &event)
{
    this->controller.terminate();
}

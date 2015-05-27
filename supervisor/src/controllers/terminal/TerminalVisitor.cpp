#include "TerminalVisitor.hpp"

#include <iostream>

#include "TerminalModule.hpp"

#include "events/Terminate.hpp"
#include "events/SendMessage.hpp"

namespace events = tin::controllers::terminal::events;

tin::controllers::terminal::TerminalVisitor::TerminalVisitor(tin::controllers::terminal::TerminalModule& controller):
controller(controller)
{}

void tin::controllers::terminal::TerminalVisitor::visit(events::Terminate &evt)
{
    this->controller.terminate();
}


void tin::controllers::terminal::TerminalVisitor::visit(events::SendMessage &evt)
{
    this->controller.send_message(evt.jsonPtr->dump().c_str());
}
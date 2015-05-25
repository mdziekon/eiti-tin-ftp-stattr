#include "TerminalVisitor.hpp"

#include <iostream>

#include "TerminalModule.hpp"

#include "events/Terminate.hpp"

namespace events = tin::controllers::terminal::events;

tin::controllers::terminal::TerminalVisitor::TerminalVisitor(tin::controllers::terminal::TerminalModule& controller):
controller(controller)
{}

void tin::controllers::terminal::TerminalVisitor::visit(events::Terminate &evt)
{
    this->controller.terminate();
}
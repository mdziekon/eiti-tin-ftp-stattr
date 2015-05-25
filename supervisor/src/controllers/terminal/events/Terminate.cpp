#include "Terminate.hpp"

#include "../TerminalVisitor.hpp"

using tin::controllers::terminal::events::Terminate;

void Terminate::accept(tin::controllers::terminal::TerminalVisitor& visitor)
{
    visitor.visit(*this);
}

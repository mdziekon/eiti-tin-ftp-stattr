#include "JSONRequestReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::JSONRequestReceived;

JSONRequestReceived::JSONRequestReceived(bool fromTerminal, const tin::utils::json::ptr& jsonPtr, unsigned int connectionID):
fromTerminal(fromTerminal), jsonPtr(jsonPtr), connectionID(connectionID)
{}

void JSONRequestReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

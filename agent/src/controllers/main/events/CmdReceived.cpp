#include "CmdReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::CmdReceived;

CmdReceived::CmdReceived(const tin::utils::json::ptr& jsonPtr):
jsonPtr(jsonPtr)
{}

void CmdReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

#include "CmdReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::CmdReceived;

CmdReceived::CmdReceived(const std::shared_ptr<json>& jsonPtr):
jsonPtr(jsonPtr)
{}

void CmdReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

#include "CmdResponseReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::CmdResponseReceived;

CmdResponseReceived::CmdResponseReceived(
    const std::string& ip,
    const unsigned int& port,
    const tin::utils::json::ptr& jsonPtr
):
ip(ip),
port(port),
jsonPtr(jsonPtr)
{}

void CmdResponseReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

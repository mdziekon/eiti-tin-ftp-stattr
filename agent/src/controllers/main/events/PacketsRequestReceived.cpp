#include "PacketsRequestReceived.hpp"

#include "../MainVisitor.hpp"

using namespace tin::controllers::main::events;

void PacketsRequestReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

PacketsRequestReceived::PacketsRequestReceived(const tin::utils::json::ptr& jsonPtr):
jsonPtr(jsonPtr)
{}
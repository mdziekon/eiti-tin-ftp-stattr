#include "TerminateNetworkManager.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::TerminateNetworkManager;

void TerminateNetworkManager::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

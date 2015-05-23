#include "ServerConnectionClosed.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::ServerConnectionClosed;

ServerConnectionClosed::ServerConnectionClosed(
    const unsigned int& serverConnectionID
):
serverConnectionID(serverConnectionID)
{}

void ServerConnectionClosed::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

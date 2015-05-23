#include "NewServerConnection.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::NewServerConnection;

NewServerConnection::NewServerConnection(
    const unsigned int& serverConnectionID
):
serverConnectionID(serverConnectionID)
{}

void NewServerConnection::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

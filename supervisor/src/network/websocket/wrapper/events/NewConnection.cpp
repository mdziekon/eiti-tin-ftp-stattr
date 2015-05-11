#include "NewConnection.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::NewConnection;

NewConnection::NewConnection(
    websocketpp::connection_hdl& connectionHandler
):
connectionHandler(connectionHandler)
{}

void NewConnection::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

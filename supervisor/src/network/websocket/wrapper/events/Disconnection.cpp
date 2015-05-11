#include "Disconnection.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::Disconnection;

Disconnection::Disconnection(
    websocketpp::connection_hdl& connectionHandler
):
connectionHandler(connectionHandler)
{}

void Disconnection::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

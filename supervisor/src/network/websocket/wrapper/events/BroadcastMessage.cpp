#include "BroadcastMessage.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::BroadcastMessage;

BroadcastMessage::BroadcastMessage(
    const std::string& message,
    const websocketpp::frame::opcode::value& method
):
message(message),
method(method)
{}

void BroadcastMessage::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

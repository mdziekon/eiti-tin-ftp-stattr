#include "OutputMessage.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::OutputMessage;

OutputMessage::OutputMessage(
    const unsigned int& connectionID,
    const std::string& message,
    const websocketpp::frame::opcode::value& method
):
connectionID(connectionID),
message(message),
method(method)
{}

void OutputMessage::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

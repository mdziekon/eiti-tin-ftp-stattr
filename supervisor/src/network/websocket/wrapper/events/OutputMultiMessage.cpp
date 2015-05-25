#include "OutputMultiMessage.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::OutputMultiMessage;

OutputMultiMessage::OutputMultiMessage(
    const std::vector<unsigned int>& connectionIDs,
    const std::string& message,
    const websocketpp::frame::opcode::value& method
):
connectionIDs(connectionIDs),
message(message),
method(method)
{}

void OutputMultiMessage::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

#include "MessageReceived.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::MessageReceived;

MessageReceived::MessageReceived(
    const unsigned int& serverConnectionID,
    const std::string& message
):
serverConnectionID(serverConnectionID),
message(message)
{}

void MessageReceived::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

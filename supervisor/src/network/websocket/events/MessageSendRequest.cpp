#include "MessageSendRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::MessageSendRequest;

MessageSendRequest::MessageSendRequest(
    const unsigned int& serverConnectionID
):
serverConnectionID(serverConnectionID)
{}

void MessageSendRequest::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

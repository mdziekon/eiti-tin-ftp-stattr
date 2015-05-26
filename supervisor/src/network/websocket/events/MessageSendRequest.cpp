#include "MessageSendRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::MessageSendRequest;

MessageSendRequest::MessageSendRequest(
    const unsigned int& serverConnectionID,
    const tin::utils::json::ptr& jsonPtr
):
serverConnectionID(serverConnectionID),
jsonPtr(jsonPtr)
{}

void MessageSendRequest::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

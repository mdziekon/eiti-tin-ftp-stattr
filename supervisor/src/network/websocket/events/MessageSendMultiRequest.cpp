#include "MessageSendMultiRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::MessageSendMultiRequest;

MessageSendMultiRequest::MessageSendMultiRequest(
    const std::vector<unsigned int>& serverConnectionIDs
):
serverConnectionIDs(serverConnectionIDs)
{}

void MessageSendMultiRequest::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

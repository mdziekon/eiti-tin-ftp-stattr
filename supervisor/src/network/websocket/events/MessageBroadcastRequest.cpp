#include "MessageBroadcastRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::websocket::events::MessageBroadcastRequest;

MessageBroadcastRequest::MessageBroadcastRequest(const std::string& message):
message(message)
{}

void MessageBroadcastRequest::accept(tin::network::websocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

#include "MessageReceived.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::MessageReceived;

MessageReceived::MessageReceived(
    const std::string& message
):
message(message)
{}

void MessageReceived::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

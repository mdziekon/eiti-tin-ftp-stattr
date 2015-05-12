#include "OutcomingMessage.hpp"

#include "../ServerVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::OutcomingMessage;

OutcomingMessage::OutcomingMessage(const std::string& message):
message(message)
{}

void OutcomingMessage::accept(tin::network::bsdsocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

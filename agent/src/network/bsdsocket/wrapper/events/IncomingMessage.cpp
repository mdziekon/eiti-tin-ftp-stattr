#include "IncomingMessage.hpp"

#include "../ServerVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::IncomingMessage;

IncomingMessage::IncomingMessage(const std::string& message):
message(message)
{}

void IncomingMessage::accept(tin::network::bsdsocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

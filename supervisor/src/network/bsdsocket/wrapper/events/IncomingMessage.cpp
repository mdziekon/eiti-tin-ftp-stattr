#include "IncomingMessage.hpp"

#include "../ClientVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::IncomingMessage;

IncomingMessage::IncomingMessage(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message
):
ip(ip),
port(port),
message(message)
{}

void IncomingMessage::accept(tin::network::bsdsocket::wrapper::ClientVisitor& i)
{
    i.visit(*this);
}

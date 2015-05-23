#include "OutcomingMessage.hpp"

#include "../ClientVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::OutcomingMessage;

OutcomingMessage::OutcomingMessage(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message,
    const bool& waitForResponse
):
ip(ip),
port(port),
message(message),
waitForResponse(waitForResponse)
{}

void OutcomingMessage::accept(tin::network::bsdsocket::wrapper::ClientVisitor& i)
{
    i.visit(*this);
}

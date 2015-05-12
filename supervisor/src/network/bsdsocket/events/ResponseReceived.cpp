#include "ResponseReceived.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::ResponseReceived;

ResponseReceived::ResponseReceived(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message
):
ip(ip),
port(port),
message(message)
{}

void ResponseReceived::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

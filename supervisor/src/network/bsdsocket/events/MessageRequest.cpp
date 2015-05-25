#include "MessageRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::MessageRequest;

MessageRequest::MessageRequest(
    const std::string& ip,
    const unsigned int& port,
    const tin::utils::json::ptr& jsonPtr,
    const bool& waitForResponse
):
ip(ip),
port(port),
jsonPtr(jsonPtr),
waitForResponse(waitForResponse)
{}

void MessageRequest::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

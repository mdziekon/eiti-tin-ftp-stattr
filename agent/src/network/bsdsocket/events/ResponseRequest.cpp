#include "ResponseRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::ResponseRequest;

ResponseRequest::ResponseRequest(const std::shared_ptr<json>& jsonPtr):
jsonPtr(jsonPtr)
{}

void ResponseRequest::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

#include "ResponseRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::ResponseRequest;

ResponseRequest::ResponseRequest():
{}

void ResponseRequest::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

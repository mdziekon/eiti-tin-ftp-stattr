#include "ConnectionTerminationRequest.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::ConnectionTerminationRequest;

void ConnectionTerminationRequest::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

#include "Terminate.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::bsdsocket::events::Terminate;

void Terminate::accept(tin::network::bsdsocket::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

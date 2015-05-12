#include "Terminate.hpp"

#include "../ServerVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::Terminate;

void Terminate::accept(tin::network::bsdsocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

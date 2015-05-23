#include "Terminate.hpp"

#include "../ClientVisitor.hpp"

using tin::network::bsdsocket::wrapper::events::Terminate;

void Terminate::accept(tin::network::bsdsocket::wrapper::ClientVisitor& i)
{
    i.visit(*this);
}

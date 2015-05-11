#include "Terminate.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::Terminate;

void Terminate::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

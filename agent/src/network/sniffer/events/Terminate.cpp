#include "Terminate.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::Terminate;

void Terminate::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

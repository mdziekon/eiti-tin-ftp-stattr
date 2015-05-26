#include "StopSniffing.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::StopSniffing;

void StopSniffing::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

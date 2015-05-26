#include "StartSniffing.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::StartSniffing;

void StartSniffing::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

#include "IsSniffing.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::IsSniffing;


void IsSniffing::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

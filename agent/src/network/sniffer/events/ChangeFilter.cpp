#include "ChangeFilter.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::ChangeFilter;

void ChangeFilter::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

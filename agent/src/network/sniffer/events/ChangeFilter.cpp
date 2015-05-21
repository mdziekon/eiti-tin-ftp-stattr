#include "ChangeFilter.hpp"

#include "../ManagerVisitor.hpp"

using tin::network::sniffer::events::ChangeFilter;

ChangeFilter::ChangeFilter(
    const std::string& device,
    const std::string& expression
):
device(device),
expression(expression)
{}

void ChangeFilter::accept(tin::network::sniffer::ManagerVisitor& visitor)
{
    visitor.visit(*this);
}

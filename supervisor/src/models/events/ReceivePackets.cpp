#include "ReceivePackets.hpp"

#include "../StatsVisitor.hpp"

using namespace tin::supervisor::models::events;

void ReceivePackets::accept(tin::supervisor::models::StatsVisitor& visitor)
{
    visitor.visit(*this);
}

ReceivePackets::ReceivePackets(const std::vector<tin::utils::json::ptr>& packets):
packets(packets)
{}

ReceivePackets::ReceivePackets(const std::vector<tin::utils::json::ptr>&& packets):
packets(packets)
{}

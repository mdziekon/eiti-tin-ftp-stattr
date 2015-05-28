#include "ReceivePackets.hpp"

#include "../StatsVisitor.hpp"

using namespace tin::supervisor::models::events;

void ReceivePackets::accept(tin::supervisor::models::StatsVisitor& visitor)
{
    visitor.visit(*this);
}

ReceivePackets::ReceivePackets(const tin::utils::json::ptr &packetsArray):
packetsArray(packetsArray)
{}

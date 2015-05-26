#include "IncomingPacket.hpp"

#include "../StatsGathererVisitor.hpp"

void tin::agent::models::events::IncomingPacket::accept(tin::agent::models::StatsGathererVisitor& visitor)
{
    visitor.visit(*this);
}

tin::agent::models::events::IncomingPacket::IncomingPacket(const tin::utils::Packet::ptr& packet):
packet(packet)
{}
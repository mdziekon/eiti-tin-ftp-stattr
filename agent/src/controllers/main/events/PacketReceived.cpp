#include "PacketReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::PacketReceived;

PacketReceived::PacketReceived(const tin::utils::packet::ptr& packet):
packet(packet)
{}

void PacketReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

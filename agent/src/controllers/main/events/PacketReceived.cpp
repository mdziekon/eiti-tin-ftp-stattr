#include "PacketReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::PacketReceived;

PacketReceived::PacketReceived(const tin::utils::Packet::ptr& packet):
pac(packet)
{}

void PacketReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

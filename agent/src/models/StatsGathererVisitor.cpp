#include "StatsGathererVisitor.hpp"

#include "StatsGatherer.hpp"
#include "events/IncomingPacket.hpp"
#include "events/RequestPackets.hpp"

using namespace tin::agent::models;

StatsGathererVisitor::StatsGathererVisitor(StatsGatherer& statsGatherer):
statsGatherer(statsGatherer)
{}

void StatsGathererVisitor::visit(const events::IncomingPacket& event)
{
    this->statsGatherer.incomingPacket(event.packet);
}

void StatsGathererVisitor::visit(const events::RequestPackets& event)
{
    this->statsGatherer.fetchPackets();
}
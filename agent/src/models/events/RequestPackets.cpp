#include "RequestPackets.hpp"

#include "../StatsGathererVisitor.hpp"

using namespace tin::agent::models::events;

void tin::agent::models::events::RequestPackets::accept(tin::agent::models::StatsGathererVisitor& visitor)
{
    visitor.visit(*this);
}

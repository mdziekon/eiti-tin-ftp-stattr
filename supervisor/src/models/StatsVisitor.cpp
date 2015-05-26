#include "StatsVisitor.hpp"

#include "Stats.hpp"

#include <arpa/inet.h>
#include <string>
#include <sys/types.h>

#include "events/ReceivePackets.hpp"
#include "events/RequestAnalytics.hpp"

#include "../controllers/main/events/NetworkRequest.hpp"
#include "../controllers/main/events/WebsocketBroadcastRequest.hpp"

using namespace tin::supervisor::models;

void StatsVisitor::visit(events::ReceivePackets& event)
{
    for(auto jsonPtr: event.packets) {
        this->stats.packetsByTimestamp.insert(
            std::pair<u_int32_t, tin::utils::json::ptr>(
                (*jsonPtr)["timestamp"].get<u_int32_t>(),
                jsonPtr
            )
        );
        this->stats.packetsBySourceIP.insert(
            std::pair<u_int32_t, tin::utils::json::ptr>(
                (*jsonPtr)["sourceIP"].get<u_int32_t>(),
                jsonPtr
            )
        );
        this->stats.packetsByDestinationIP.insert(
            std::pair<u_int32_t, tin::utils::json::ptr>(
                (*jsonPtr)["destinationIP"].get<u_int32_t>(),
                jsonPtr
            )
        );
    }
}

void StatsVisitor::visit(events::RequestAnalytics& event)
{
    tin::utils::json::ptr computedStats;
    if((*event.requestData)["route"].get<std::string>() == "stats-per-day") {
        computedStats = this->stats.computeStatsPerDay(event.requestData);
    }
    else if((*event.requestData)["route"].get<std::string>() == "traffic-per-machine") {
        computedStats = this->stats.computeIndividualUsage(event.requestData);
    }
    
    this->stats.controllerQueue.push(
        std::make_shared<tin::controllers::main::events::WebsocketBroadcastRequest>(
            computedStats
        )
    );
}

StatsVisitor::StatsVisitor(tin::supervisor::models::Stats& stats):
stats(stats)
{}

#include "StatsVisitor.hpp"

#include "Stats.hpp"

#include <arpa/inet.h>
#include <string>
#include <sys/types.h>

#include "events/ReceivePackets.hpp"
#include "events/RequestAnalytics.hpp"

#include "../controllers/main/events/NetworkRequest.hpp"
#include "../controllers/main/events/WebClientSendRequest.hpp"

using namespace tin::supervisor::models;

void StatsVisitor::visit(events::ReceivePackets& event)
{
    for(auto jsonPtr: event.packets) {
        this->stats.packets.push_back(tin::utils::Packet((*jsonPtr)));

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
    else if((*event.requestData)["route"].get<std::string>() == "stats-per-machine") {
        computedStats = this->stats.computeIndividualUsage(event.requestData);
    }
    else {
        std::cout << "[Supervisor] Invalid or unknown analytics request received" << std::endl;
    }
    
    this->stats.controllerQueue.push(
        std::make_shared<tin::controllers::main::events::WebClientSendRequest>(
            computedStats,
            event.connectionID
        )
    );
}

StatsVisitor::StatsVisitor(tin::supervisor::models::Stats& stats):
stats(stats)
{}

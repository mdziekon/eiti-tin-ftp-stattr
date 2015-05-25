#include "StatsGatherer.hpp"

#include <netinet/in.h>
#include <iostream>

#include "events/IncomingPacket.hpp"
#include "../controllers/main/events/NetworkReply.hpp"

using namespace tin::agent::models;

void StatsGatherer::incomingPacket(const tin::utils::Packet::ptr& packet)
{
    if(packet->protocol == IPPROTO_TCP) {
        this->capturedPackets.push_back(packet);
    }
}

void StatsGatherer::fetchPackets()
{   
    nlohmann::json reply;
    u_int32_t loopIndex = 0;
        
    for(auto& packet: this->capturedPackets) {
        reply["data"][loopIndex++] = packet->serialize();
    }
    
    std::cout << "[Agent][Internal] Sending " << this->capturedPackets.size() << " packets to the network\r\n"; 
    
    this->capturedPackets.clear();
    
    this->controllerQueue.push(std::make_shared<tin::controllers::main::events::NetworkReply>(reply));
}

StatsGatherer::StatsGatherer(
    tin::agent::models::StatsGathererQueue& queue,
    tin::controllers::main::ControllerQueue& controllerQueue
):
QueueThread(queue, StatsGathererVisitor(*this)),
controllerQueue(controllerQueue)
{}
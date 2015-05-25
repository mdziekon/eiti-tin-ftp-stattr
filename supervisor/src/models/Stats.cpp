#include "Stats.hpp"

#include <arpa/inet.h>

#include "StatsVisitor.hpp"
#include "../controllers/main/events/NetworkRequest.hpp"

using namespace tin::supervisor::models;

void Stats::updateDataset()
{
    nlohmann::json json;
    
    json["cmd"] = "fetch_packets";
    
    this->controllerQueue.push(
        std::make_shared<tin::controllers::main::events::NetworkRequest>(
            "localhost",
            3333,
            std::make_shared<nlohmann::json>(json)
        )
    );
}

Stats::Stats(
    tin::supervisor::models::StatsQueue& statsQueue,
    tin::controllers::main::ControllerQueue& controllerQueue
): 
QueueThread< tin::supervisor::models::Event, tin::supervisor::models::StatsVisitor >(statsQueue, StatsVisitor(*this)),
controllerQueue(controllerQueue)
{}
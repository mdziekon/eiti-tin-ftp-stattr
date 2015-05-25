#include "Stats.hpp"

#include <arpa/inet.h>
#include <thread>

#include "StatsVisitor.hpp"
#include "../controllers/main/events/NetworkRequest.hpp"

using namespace tin::supervisor::models;

std::atomic<u_int32_t> Stats::maxID(1);

void Stats::updateDataset()
{
    nlohmann::json json;
    
    json["action"] = "fetch_packets";
    json["requestor_id"] = this->id;
    
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
controllerQueue(controllerQueue),
id(Stats::maxID++)
{}
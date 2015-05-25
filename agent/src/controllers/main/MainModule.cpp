#include "MainModule.hpp"

#include <utility>

#include "Event.hpp"
#include "../../network/bsdsocket/events/MessageReceived.hpp"
#include "../../network/bsdsocket/events/ResponseRequest.hpp"

tin::controllers::main::MainModule::MainModule(
    tin::controllers::main::ControllerQueue &incomingQueue,
    tin::network::bsdsocket::ManagerQueue &networkManagerQueue,
    tin::agent::models::StatsGathererQueue& statsGathererQueue
):
QueueThread(incomingQueue, MainVisitor(*this)),
networkManagerQueue(networkManagerQueue),
statsGathererQueue(statsGathererQueue)
{}

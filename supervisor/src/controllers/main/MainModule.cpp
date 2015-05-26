#include "MainModule.hpp"

#include <utility>

#include "Event.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageSendMultiRequest.hpp"
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"

tin::controllers::main::MainModule::MainModule(
    tin::controllers::main::ControllerQueue &incomingQueue,
    tin::network::websocket::ManagerQueue &networkManagerQueue,
    tin::network::bsdsocket::ManagerQueue& bsdManagerQueue,
    tin::supervisor::models::StatsQueue& statsQueue
):
QueueThread(incomingQueue, MainVisitor(*this)),
statsQueue(statsQueue),
networkManagerQueue(networkManagerQueue),
bsdManagerQueue(bsdManagerQueue)
{}

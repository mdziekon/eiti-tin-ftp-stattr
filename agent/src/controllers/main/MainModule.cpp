#include "MainModule.hpp"

#include <utility>

#include "Event.hpp"
#include "../../network/bsdsocket/events/MessageReceived.hpp"
#include "../../network/bsdsocket/events/ResponseRequest.hpp"
#include "../../network/sniffer/events/ChangeFilter.hpp"

tin::controllers::main::MainModule::MainModule(
    tin::controllers::main::ControllerQueue &incomingQueue,
    tin::network::bsdsocket::ManagerQueue &networkManagerQueue,
    tin::network::sniffer::ManagerQueue &snifferManagerQueue
):
QueueThread(incomingQueue, MainVisitor(*this)),
networkManagerQueue(networkManagerQueue),
snifferManagerQueue(snifferManagerQueue)
{}

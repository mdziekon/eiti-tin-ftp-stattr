#include "Manager.hpp"

#include "../../controllers/main/typedefs.hpp"
#include "../../controllers/main/events/PacketReceived.hpp"

using namespace tin::network::sniffer;

Manager::Manager(
    tin::network::sniffer::ManagerQueue& incomingQueue,
    tin::controllers::main::ControllerQueue& controllerQueue,
    std::string device,
    std::string expression
):
QueueThread(incomingQueue, ManagerVisitor(*this)),
sniffer(device, expression),
controllerQueue(controllerQueue)
{
    this->sniffer.run();

    this->sniffer.attachPacketReceivedHandler([this](const tin::utils::Packet::ptr& packet) {
        this->controllerQueue.push(
            tin::controllers::main::EventPtr(new tin::controllers::main::events::PacketReceived(packet))
        );
    });
}

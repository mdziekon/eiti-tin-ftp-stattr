using tin::network::trafficcapture::SnifferManager;

SnifferManager::SnifferManager(
	tin::controllers::main::ControllerQueue& controllerQueue,
    std::string device,
    std::string expression
):
Sniffer(device, expression),
controllerQueue(controllerQueue)
{
    this->sniffer.run();

    this->sniffer.attachPacketReceivedHandler([this](const tin::utils::packet::ptr& packet) {
        this->controllerQueue.push(
            EventPtr(new tin::controllers::main::events::PacketReceived(packet))
        );
    });
}

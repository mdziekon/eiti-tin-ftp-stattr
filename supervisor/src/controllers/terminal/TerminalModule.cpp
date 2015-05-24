#include "TerminalModule.hpp"

#include <utility>

#include "Event.hpp"

tin::controllers::terminal::TerminalModule::TerminalModule(
    tin::controllers::terminal::TerminalQueue &incomingQueue,
    tin::controllers::main::ControllerQueue &controllerQueue
):
QueueThread(incomingQueue, TerminalVisitor(*this)),
controllerQueue(controllerQueue)
{
	tcp::endpoint endpoint(tcp::v4(), 4321); // Temporary port
	terminalServer_ = new tin::controllers::terminal::TerminalServer(io_service_, endpoint);
	io_service_.run();
}

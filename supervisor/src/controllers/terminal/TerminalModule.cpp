#include "TerminalModule.hpp"

#include <utility>

#include "Event.hpp"

tin::controllers::terminal::TerminalModule::TerminalModule(
    tin::controllers::terminal::TerminalQueue &incomingQueue,
    tin::controllers::main::ControllerQueue &controllerQueue,
	boost::asio::io_service& io_service) :	
		QueueThread(incomingQueue, TerminalVisitor(*this))
{
	terminalServer_ = new tin::controllers::terminal::TerminalServer(controllerQueue, io_service, 4321);
}

#include "TerminalModule.hpp"

#include <utility>

#include "Event.hpp"

tin::controllers::terminal::TerminalModule::TerminalModule(
    tin::controllers::terminal::TerminalQueue &incomingQueue,
    tin::controllers::main::ControllerQueue &controllerQueue,
	boost::asio::io_service& io_service) :	
		QueueThread(incomingQueue, TerminalVisitor(*this))
{
	terminalServerPtr_.reset(
		new tin::controllers::terminal::TerminalServer(
			controllerQueue,
			io_service,
			4321));
}

void tin::controllers::terminal::TerminalModule::send_message(const char* message)
{
	terminalServerPtr_->send_message(message);
}
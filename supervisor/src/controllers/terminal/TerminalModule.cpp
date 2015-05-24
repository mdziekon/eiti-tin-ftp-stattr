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
	boost::asio::io_service io_service;
	boost::asio::io_service::work work(io_service);
	
	std::thread t([&io_service]() { io_service.run(); });
	terminalServer_ = new tin::controllers::terminal::TerminalServer(io_service, 4321);
	// std::cout << "Creating terminal session!" << std::endl;
	t.join();
}

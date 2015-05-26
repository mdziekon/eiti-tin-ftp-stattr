#include "TerminalServer.hpp"
#include <thread>

tin::controllers::terminal::TerminalServer::TerminalServer(
  tin::controllers::main::ControllerQueue &controllerQueue,
	boost::asio::io_service& io_service,
	short port) :
	controllerQueue(controllerQueue),
	acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{
  do_accept();
}

void tin::controllers::terminal::TerminalServer::do_accept()
{
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
		{
			if(!ec)
			{
        		terminalSessionPtr_.reset(new TerminalSession(controllerQueue, std::move(socket_)));
        		terminalSessionPtr_->start();
			}
      
			do_accept();
		});
}

void tin::controllers::terminal::TerminalServer::send_message(const char* message)
{
	terminalSessionPtr_->do_write(message);
}
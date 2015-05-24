#include "TerminalServer.hpp"
#include "TerminalSession.hpp"
#include <thread>

tin::controllers::terminal::TerminalServer::TerminalServer(
	boost::asio::io_service& io_service,
	short port) :
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
        std::make_shared<TerminalSession>(std::move(socket_))->start();
			}
      
			do_accept();
		});
}
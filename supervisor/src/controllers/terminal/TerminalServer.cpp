#include "TerminalServer.hpp"
#include <thread>

tin::controllers::terminal::TerminalServer::TerminalServer(
	boost::asio::io_service& io_service,
	const tcp::endpoint& endpoint) :
	acceptor_(io_service, endpoint),
	socket_(io_service)
{
	//std::thread thread(&TerminalServer::do_accept, this);
	//thread.join();
}

void tin::controllers::terminal::TerminalServer::do_accept()
{
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
		{
			if(!ec)
			{
      			printf("I received something");
    			do_read_header();
			}

			do_accept();
		});
}


void tin::controllers::terminal::TerminalServer::do_read_header()
{
boost::asio::async_read(socket_,
    boost::asio::buffer(cmd_msg_.data(), command_message::header_length),
    [this](boost::system::error_code ec, int /*length*/)
    {
      if (!ec && cmd_msg_.decode_header())
      {
        do_read_body();
      }
      else
      {
        //room_.leave(shared_from_this());
      }
    });
}

void tin::controllers::terminal::TerminalServer::do_read_body()
{
	boost::asio::async_read(socket_,
    boost::asio::buffer(cmd_msg_.body(), cmd_msg_.body_length()),
    [this](boost::system::error_code ec, int /*length*/)
    {
      if (!ec)
      {
      	respond();
        do_read_header();
      }
      else
      {
        //room_.leave(shared_from_this());
      }
    });
}

void tin::controllers::terminal::TerminalServer::respond()
{
boost::asio::async_write(socket_,
    boost::asio::buffer(cmd_msg_.data(),
      cmd_msg_.length()),
    [this](boost::system::error_code ec, int /*length*/)
    {
      if (!ec)
      {

      }
      else
      {
        //room_.leave(shared_from_this());
      }
    });
}
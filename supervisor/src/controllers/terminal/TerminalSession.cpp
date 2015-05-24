#include "TerminalSession.hpp"

tin::controllers::terminal::TerminalSession::TerminalSession(tcp::socket socket) :
	socket_(std::move(socket))
{
}

void tin::controllers::terminal::TerminalSession::start()
{
	do_read();
}

void tin::controllers::terminal::TerminalSession::do_read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this, self](boost::system::error_code ec, int length)
		{
			if(!ec)
			{
  				std::cout << "Terminal sent command: " << data_ << std::endl;
				do_write(length);
			}
		});
}

void tin::controllers::terminal::TerminalSession::do_write(int length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
		[this, self](boost::system::error_code ec, int /*length*/)
		{
			if(!ec)
			{
				do_read();
			}
		});
}
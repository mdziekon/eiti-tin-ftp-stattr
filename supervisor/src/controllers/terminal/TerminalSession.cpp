#include "TerminalSession.hpp"
#include "../main/events/JSONRequestReceived.hpp"


tin::controllers::terminal::TerminalSession::TerminalSession(
        tin::controllers::main::ControllerQueue &controllerQueue,
        tcp::socket socket) :
	controllerQueue(controllerQueue),
	socket_(std::move(socket))
{
}

void tin::controllers::terminal::TerminalSession::start()
{
	do_read_header();
}

void tin::controllers::terminal::TerminalSession::do_read_header()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(msg_.data(), terminal_message::header_length),
		[this, self](boost::system::error_code ec, int)
		{				
			if(!ec && msg_.decode_header())
			{
				do_read_body();
			} else {
				socket_.close();
			}
			
		});
}

void tin::controllers::terminal::TerminalSession::do_read_body()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(msg_.body(), msg_.body_length()),
		[this, self](boost::system::error_code ec, int)
		{
			if(!ec)
			{
				try
				{
				controllerQueue.push(
            	std::make_shared<tin::controllers::main::events::JSONRequestReceived>(
	                true,
	                std::make_shared<nlohmann::json>(
	                    nlohmann::json::parse(std::string(msg_.body(), msg_.body_length()))
	                ),
	                0
            	));
			}
			catch(std::exception& e)
			{
				std::cout << "[[[[EXCEPTION]]]" << std::endl;
					std::cout << msg_.body() << std::endl;
					std::cout << msg_.body_length() << std::endl;
			}

				do_read_header();
			} else {
				socket_.close();
			}
		});	
}

void tin::controllers::terminal::TerminalSession::do_write(const char* message)
{
	terminal_message msg;
	msg.body_length(std::strlen(message));
	std::memcpy(msg.body(), message, msg.body_length());
	msg.encode_header();

	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(msg.data(), msg.length()),
		[this, self](boost::system::error_code ec, int /*length*/)
		{
			if(!ec)
			{
				std::cout << "> Sending data to terminal... " << std::endl;
			}
		});
}

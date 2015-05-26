#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "terminal_message.hpp"

using boost::asio::ip::tcp;

enum { max_length = 1024 };
char data[max_length];

class Client
{
public:
	Client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator) :
		socket_(io_service),
		io_service_(io_service)
	{
		connect(endpoint_iterator);
	}

	void connect(tcp::resolver::iterator endpoint_iterator)
	{
	    boost::asio::async_connect(socket_, endpoint_iterator,
	        [this](boost::system::error_code ec, tcp::resolver::iterator)
	        {
	        	if(!ec)
	        	{
	        		std::cout << "> Connected to supervisor." << std::endl;
	        		read_header();
	        	}
	        });
	}

	void write(char* data)
	{
		terminal_message msg;
		msg.body_length(std::strlen(data));
		std::memcpy(msg.body(), data, msg.body_length());
		msg.encode_header();

		boost::asio::write(socket_, boost::asio::buffer(msg.data(), msg.length()));
	}

	void close()
	{
	    io_service_.post([this]() { socket_.close(); });
	}

	void read_header()
	{
		boost::asio::async_read(socket_, boost::asio::buffer(reply_msg_.data(), terminal_message::header_length),
			[this](boost::system::error_code ec, int)
			{
				if(!ec && reply_msg_.decode_header())
				{
					read_body();
				} else {
					socket_.close();
				}
			});			
	}

	void read_body()
	{
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(reply_msg_.body(), reply_msg_.body_length()),
			[this](boost::system::error_code ec, int )
			{
				if(!ec)
				{
					std::cout.write(reply_msg_.body(), reply_msg_.body_length());
					std::cout << std::endl;
					read_header();
				} else {
					socket_.close();
				}
			});		
	}

private:
	terminal_message reply_msg_;
	tcp::socket socket_;
	boost::asio::io_service& io_service_;
};

int main(int argc, char* argv[])
{
	try
	{
		char host[] = "localhost";
		char port[] = "4321";

		std::cout << "Terminal sessions started." << std::endl;

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ host, port });
		Client c(io_service, endpoint_iterator);

		std::thread t([&io_service]() { io_service.run(); });
		
		while(std::cin.getline(data, max_length))
		{
			c.write(data);
		}

		c.close();
		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "command_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque <command_message> command_message_queue;

class terminal_client
{
public:
	terminal_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
	: io_service_(io_service),
		socket_(io_service)
	{
		do_connect(endpoint_iterator);		
	}

	void write(const command_message& msg)
	{
		io_service_.post(
			[this, msg] ()
			{
				bool write_in_progress = !msgs_.empty();
				msgs_.push_back(msg);
				if(!write_in_progress)
				{
					do_write();
				}
			});
	}

	void close()
	{
		io_service_.post(
			[this] ()
			{
				socket_.close();
			});
	}

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		boost::asio::async_connect(socket_, endpoint_iterator,
			[this](boost::system::error_code ec, tcp::resolver::iterator)
			{
				if(!ec)
				{
					// read_ACC;
				}
			});
	}

	void do_write()
	{
		boost::asio::async_write(socket_,
			boost::asio::buffer(msgs_.front().data(),
				msgs_.front().length()),
			[this](boost::system::error_code ec, std::size_t /* ?? */)
		{
			if(!ec)
			{
				msgs_.pop_front();
				if(!msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				socket_.close();
			}
		});
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	command_message command_msg_;
	command_message_queue msgs_;
};

int main(int argc, char* argv[])
{
	try
	{
		if(argc != 3)
		{
			std::cerr << "Usage: <host> <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
		terminal_client client(io_service, endpoint_iterator);

		std::thread t([&io_service]() { io_service.run(); });

		char line[command_message::max_body_length + 1];

		while(std::cin.getline(line, command_message::max_body_length + 1))
		{
			command_message msg;
			msg.body_length(std::strlen(line));
			std::memcpy(msg.body(), line, msg.body_length());
			msg.encode_header();
			client.write(msg);
		}

		client.close();
		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
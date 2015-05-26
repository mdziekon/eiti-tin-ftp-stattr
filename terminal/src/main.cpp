#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "terminal_message.hpp"
#include "../../supervisor/src/utils/JSON.hpp"

using boost::asio::ip::tcp;


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

	void write(const char* data)
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

bool parse_command(char* data);
enum { max_length = 1024 };
char data[max_length];
std::shared_ptr<Client> clientPtr;


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
		clientPtr.reset(new Client(io_service, endpoint_iterator));

		std::thread t([&io_service]() { io_service.run(); });
		
		while(std::cin.getline(data, max_length))
		{
			if(!parse_command(data))
				break;
		}

		clientPtr->close();
		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

bool parse_command(char* data)
{
	std::vector <std::string> tokenList;
	std::string s(data);
	boost::split(tokenList, s, boost::is_any_of("\t "));


	if(tokenList[0].compare("-quit") == 0)
	{
		return false;
	}
	else if (tokenList[0].compare("-test") == 0)
	{
		clientPtr->write(data);
	}
	else if (tokenList[0].compare("-help") == 0)
	{
		std::cout << "  -quit - to exit terminal\n"
			<< "  -test - to get test response from the supervisor\n"
			<< "  -add <name> <ip> <port> - to get test response from the supervisor\n"
			<< "  -remove <id> - to get test response from the supervisor\n"
			<< "  -get <id> - to get info about machine\n";
	}
	else if(tokenList[0].compare("-add") == 0)
	{
		if(tokenList.size() != 4)
			std::cout << "ERROR. Prameters count: " << tokenList.size() - 1<< ", expected 3.\n";
		else
		{
			nlohmann::json temp;
			temp["route"] = "machine";
			temp["type"] = "POST";
			temp["data"]["name"] = tokenList[1];
			temp["data"]["ip"] = tokenList[2];
			temp["data"]["port"] = atoi(tokenList[3].c_str());

			clientPtr->write(temp.dump().c_str());
		}
	} 
	else if(tokenList[0].compare("-remove") == 0)
	{
		if(tokenList.size() != 2)
			std::cout << "ERROR. Prameters count: " << tokenList.size() - 1<< ", expected 1.\n";
		else
		{
			nlohmann::json temp;
			temp["route"] = "machine/" + tokenList[1];
			temp["type"] = "DELETE";
		
			clientPtr->write(temp.dump().c_str());
		}
	}
	else if(tokenList[0].compare("-get") == 0)
	{
		if(tokenList.size() != 2)
			std::cout << "ERROR. Prameters count: " << tokenList.size() - 1<< ", expected 1.\n";
		else
		{
			nlohmann::json temp;
			temp["route"] = "machine/" + tokenList[1];
			temp["type"] = "GET";
		
			clientPtr->write(temp.dump().c_str());
		}
	}
	else
	{
		std::cout << "Bad command. Type '-help' to display command list." << std::endl;
	}
	return true;
}
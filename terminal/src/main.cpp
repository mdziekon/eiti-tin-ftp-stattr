#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };
char data[max_length];

int main(int argc, char* argv[])
{
	try
	{
		char host[] = "localhost";
		char port[] = "4321";

		std::cout << "Terminal sessions started." << std::endl;

		boost::asio::io_service io_service;

		tcp::socket socket(io_service);
		tcp::resolver resolver(io_service);
		
		auto endpoint_iterator = resolver.resolve({ host, port });
    	boost::asio::connect(socket, endpoint_iterator);

		std::thread t([&io_service]() { io_service.run(); });
		
		// char reply[max_length];
		
		while(std::cin.getline(data, max_length))
		{
			boost::asio::write(socket, boost::asio::buffer(data, std::strlen(data)));
			
			/*
			boost::asio::async_read(socket, boost::asio::buffer(reply, max_length),
				[](boost::system::error_code ec, int )
			{
				if(!ec)
				{
					std::cout << "Server replied!";
				}	
			});
			*/
		}

		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
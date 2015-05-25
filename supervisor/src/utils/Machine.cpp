#include "Machine.hpp"

tin::utils::Machine::Machine(std::string ip, unsigned int port)
    	{
    		this->id = incCounter();
    		this->ip = ip;
    		this->port = port;
    	};

    	
tin::utils::Machine::Machine(const int &id, const std::string &ip, const unsigned int &port)
    	: id(id), ip(ip), port(port) {};

int tin::utils::Machine::incCounter()
{
	static int counter = 0;
	counter++;
	return counter;

}
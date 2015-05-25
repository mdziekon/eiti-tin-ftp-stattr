#ifndef TIN_UTILS_MACHINE_HPP
#define TIN_UTILS_MACHINE_HPP
#include <string>

namespace tin { namespace utils
{
    struct Machine {

    	Machine(std::string ip, std::string name, unsigned int port);
    	Machine(const int &id, const std::string &ip, const unsigned int &port);
        int incCounter();
    	int id;
        std::string name;
    	std::string ip;
        unsigned int port;
        std::string filter; 
        std::string status;
        int lastSynchronization;
    };
}}


#endif	/* TIN_UTILS_MACHINE_HPP */


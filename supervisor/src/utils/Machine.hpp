#ifndef TIN_UTILS_MACHINE_HPP
#define TIN_UTILS_MACHINE_HPP

namespace tin { namespace utils
{
    struct Machine {
    	static int machineId = 0;
    	std::string ip;
        unsigned int port;
        std::string filter; 
        std::string stat;
        int lastSynchronization;
    }
}}

#endif	/* TIN_UTILS_MACHINE_HPP */


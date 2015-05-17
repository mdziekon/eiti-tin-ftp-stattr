#ifndef TIN_NETWORK_SNIFFER_MANAGER_HPP
#define TIN_NETWORK_SNIFFER_MANAGER_HPP

#include "Sniffer.hpp"

#include "../../controllers/main/typedefs.hpp"

namespace tin { namespace network { namespace sniffer
{
    class SnifferManager
    {
    public:
        SnifferManager(tin::controllers::main::ControllerQueue& controllerQueue, std::string device, std::string expression);
    private:
        tin::network::sniffer::Sniffer sniffer;
        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif  /* TIN_NETWORK_SNIFFER_MANAGER_HPP */

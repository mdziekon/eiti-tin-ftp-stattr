#ifndef TIN_NETWORK_TRAFFICCAPTURE_MANAGER_HPP
#define TIN_NETWORK_TRAFFICCAPTURE_MANAGER_HPP

#include "Sniffer.hpp"
#include "../../controllers/main/typedefs.hpp"
#include <iostream>

namespace tin { namespace network { namespace trafficcapture
{
    class SnifferManager
    {
    public:
        SnifferManager(tin::controllers::main::ControllerQueue& controllerQueue, std::string device, std::string expression);
    private:
        tin::network::trafficcapture::Sniffer sniffer;
        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif  /* TIN_NETWORK_TRAFFICCAPTURE_MANAGER_HPP */
#ifndef TIN_NETWORK_SNIFFER_MANAGER_HPP
#define TIN_NETWORK_SNIFFER_MANAGER_HPP

#include "typedefs.hpp"
#include "../../controllers/main/typedefs.hpp"

#include "Event.hpp"
#include "ManagerVisitor.hpp"
#include "Sniffer.hpp"

#include "../../utils/QueueThread.hpp"

namespace tin { namespace network { namespace sniffer
{
    class Manager: public tin::utils::QueueThread
    <tin::network::sniffer::Event, tin::network::sniffer::ManagerVisitor>
    {
        friend class tin::network::sniffer::ManagerVisitor;
    public:
        Manager(
            tin::network::sniffer::ManagerQueue& incomingQueue,
            tin::controllers::main::ControllerQueue& controllerQueue,
            std::string device,
            std::string expression
        );
    private:
        tin::network::sniffer::Sniffer sniffer;
        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif  /* TIN_NETWORK_SNIFFER_MANAGER_HPP */

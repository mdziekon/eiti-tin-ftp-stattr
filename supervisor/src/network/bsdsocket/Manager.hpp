#ifndef TIN_NETWORK_BSDSOCKET_MANAGER_HPP
#define TIN_NETWORK_BSDSOCKET_MANAGER_HPP

#include "typedefs.hpp"
#include "../../controllers/main/typedefs.hpp"

#include "Event.hpp"
#include "ManagerVisitor.hpp"
#include "wrapper/Client.hpp"

#include "../../utils/QueueThread.hpp"

namespace tin { namespace network { namespace bsdsocket
{
    class Manager: public tin::utils::QueueThread
    <tin::network::bsdsocket::Event, tin::network::bsdsocket::ManagerVisitor>
    {
        friend class tin::network::bsdsocket::ManagerVisitor;
    public:
        Manager(
            tin::network::bsdsocket::ManagerQueue& incomingQueue,
            tin::controllers::main::ControllerQueue& controllerQueue
        );
    private:
        tin::network::bsdsocket::wrapper::Client client;

        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif  /* TIN_NETWORK_BSDSOCKET_MANAGER_HPP */


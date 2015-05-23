#ifndef TIN_NETWORK_WEBSOCKET_MANAGER_HPP
#define	TIN_NETWORK_WEBSOCKET_MANAGER_HPP

#include "typedefs.hpp"
#include "../../controllers/main/typedefs.hpp"

#include "Event.hpp"
#include "ManagerVisitor.hpp"
#include "wrapper/Server.hpp"

#include "../../utils/QueueThread.hpp"

namespace tin { namespace network { namespace websocket
{
    class Manager: public tin::utils::QueueThread
    <tin::network::websocket::Event, tin::network::websocket::ManagerVisitor>
    {
        friend class tin::network::websocket::ManagerVisitor;
    public:
        Manager(
            tin::network::websocket::ManagerQueue& incomingQueue,
            tin::controllers::main::ControllerQueue& controllerQueue,
            const unsigned int& serverPortNo
        );
    private:
        tin::network::websocket::wrapper::Server server;

        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif	/* TIN_NETWORK_WEBSOCKET_MANAGER_HPP */


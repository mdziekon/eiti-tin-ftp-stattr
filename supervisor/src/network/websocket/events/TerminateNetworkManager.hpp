#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_TERMINATENETWORKMANAGER_HPP
#define	TIN_NETWORK_WEBSOCKET_EVENTS_TERMINATENETWORKMANAGER_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct TerminateNetworkManager: public tin::network::websocket::Event
    {
        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_TERMINATENETWORKMANAGER_HPP */


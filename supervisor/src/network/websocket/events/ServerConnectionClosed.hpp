#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_SERVERCONNECTIONCLOSED_HPP
#define	TIN_NETWORK_WEBSOCKET_EVENTS_SERVERCONNECTIONCLOSED_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct ServerConnectionClosed: public tin::network::websocket::Event
    {
        unsigned int serverConnectionID;

        ServerConnectionClosed(const unsigned int& serverConnectionID);

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_SERVERCONNECTIONCLOSED_HPP */


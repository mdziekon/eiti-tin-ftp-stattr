#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_NEWSERVERCONNECTION_HPP
#define	TIN_NETWORK_WEBSOCKET_EVENTS_NEWSERVERCONNECTION_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct NewServerConnection: public tin::network::websocket::Event
    {
        unsigned int serverConnectionID;

        NewServerConnection(const unsigned int& serverConnectionID);

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_NEWSERVERCONNECTION_HPP */


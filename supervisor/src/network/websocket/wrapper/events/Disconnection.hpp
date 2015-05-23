#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_DISCONNECTION_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_DISCONNECTION_HPP

#include <websocketpp/server.hpp>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct Disconnection: public tin::network::websocket::wrapper::Event
    {
        websocketpp::connection_hdl connectionHandler;

        Disconnection(websocketpp::connection_hdl& connectionHandler);
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_DISCONNECTION_HPP */


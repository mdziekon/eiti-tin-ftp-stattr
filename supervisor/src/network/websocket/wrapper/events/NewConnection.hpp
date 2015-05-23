#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_NEWCONNECTION_HPP
#define TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_NEWCONNECTION_HPP

#include <websocketpp/server.hpp>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct NewConnection: public tin::network::websocket::wrapper::Event
    {
        websocketpp::connection_hdl connectionHandler;

        NewConnection(websocketpp::connection_hdl& connectionHandler);
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif /* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_NEWCONNECTION_HPP */

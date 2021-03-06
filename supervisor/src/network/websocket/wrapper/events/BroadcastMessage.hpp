#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_BROADCASTMESSAGE_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_BROADCASTMESSAGE_HPP

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct BroadcastMessage: public tin::network::websocket::wrapper::Event
    {
        const std::string message;
        const websocketpp::frame::opcode::value method;

        BroadcastMessage(
            const std::string& message,
            const websocketpp::frame::opcode::value& method
        );
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_BROADCASTMESSAGE_HPP */


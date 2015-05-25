#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_MESSAGE_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_MESSAGE_HPP

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct Message: public tin::network::websocket::wrapper::Event
    {
        websocketpp::connection_hdl connectionHandler;
        websocketpp::server<websocketpp::config::asio>::message_ptr message;

        Message(
            websocketpp::connection_hdl& connectionHandler,
            websocketpp::server<websocketpp::config::asio>::message_ptr& message
        );
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_MESSAGE_HPP */


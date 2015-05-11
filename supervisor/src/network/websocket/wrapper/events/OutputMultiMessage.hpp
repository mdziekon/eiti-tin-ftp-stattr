#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_OUTPUTMULTIMESSAGE_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_OUTPUTMULTIMESSAGE_HPP

#include <vector>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct OutputMultiMessage: public tin::network::websocket::wrapper::Event
    {
        const std::vector<unsigned int> connectionIDs;
        const std::string message;
        const websocketpp::frame::opcode::value method;

        OutputMultiMessage(
            const std::vector<unsigned int>& connectionIDs,
            const std::string& message,
            const websocketpp::frame::opcode::value& method
        );
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_OUTPUTMULTIMESSAGE_HPP */


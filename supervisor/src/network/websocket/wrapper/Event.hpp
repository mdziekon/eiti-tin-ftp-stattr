#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENT_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENT_HPP

#include "../../../utils/Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper
{
    class ServerVisitor;

    struct Event:
    public tin::utils::Event<tin::network::websocket::wrapper::ServerVisitor>
    {
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i) = 0;
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENT_HPP */


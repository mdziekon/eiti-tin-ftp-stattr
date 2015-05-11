#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_TERMINATE_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_TERMINATE_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace events
{
    struct Terminate: public tin::network::websocket::wrapper::Event
    {
        virtual void accept(tin::network::websocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_EVENTS_TERMINATE_HPP */


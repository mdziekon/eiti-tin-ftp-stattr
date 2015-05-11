#ifndef TIN_NETWORK_WEBSOCKET_EVENT_HPP
#define TIN_NETWORK_WEBSOCKET_EVENT_HPP

#include "../../utils/Event.hpp"

namespace tin { namespace network { namespace websocket
{
    class ManagerVisitor;

    struct Event: public tin::utils::Event<tin::network::websocket::ManagerVisitor>
    {
        virtual void accept(tin::network::websocket::ManagerVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_NETWORK_WEBSOCKET_EVENT_HPP */

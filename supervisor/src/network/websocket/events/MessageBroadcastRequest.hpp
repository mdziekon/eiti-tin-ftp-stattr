#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGEBROADCASTREQUEST_HPP
#define TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGEBROADCASTREQUEST_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct MessageBroadcastRequest: public tin::network::websocket::Event
    {
        const std::string message;
        
        MessageBroadcastRequest(const std::string& message);

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGEBROADCASTREQUEST_HPP */


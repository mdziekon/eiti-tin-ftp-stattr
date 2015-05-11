#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP
#define TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP

#include <memory>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct MessageSendRequest: public tin::network::websocket::Event
    {
        unsigned int serverConnectionID;

        MessageSendRequest(
            const unsigned int& serverConnectionID
        );

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP */


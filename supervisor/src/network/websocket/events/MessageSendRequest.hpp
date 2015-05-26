#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP
#define TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct MessageSendRequest: public tin::network::websocket::Event
    {
        unsigned int serverConnectionID;
        const tin::utils::json::ptr jsonPtr;

        MessageSendRequest(
            const unsigned int& serverConnectionID,
            const tin::utils::json::ptr& jsonPtr
        );

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDREQUEST_HPP */


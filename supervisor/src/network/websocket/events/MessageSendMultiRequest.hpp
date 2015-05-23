#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDMULTIREQUEST_HPP
#define TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDMULTIREQUEST_HPP

#include <memory>
#include <vector>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct MessageSendMultiRequest: public tin::network::websocket::Event
    {
        const std::vector<unsigned int> serverConnectionIDs;

        MessageSendMultiRequest(
            const std::vector<unsigned int>& serverConnectionIDs
        );

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGESENDMULTIREQUEST_HPP */


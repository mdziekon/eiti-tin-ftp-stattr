#ifndef TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGERECEIVED_HPP
#define TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGERECEIVED_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace websocket { namespace events
{
    struct MessageReceived: public tin::network::websocket::Event
    {
        unsigned int serverConnectionID;
        std::string message;

        MessageReceived(
            const unsigned int& serverConnectionID,
            const std::string& message
        );

        virtual void accept(tin::network::websocket::ManagerVisitor& visitor);
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_EVENTS_MESSAGERECEIVED_HPP */


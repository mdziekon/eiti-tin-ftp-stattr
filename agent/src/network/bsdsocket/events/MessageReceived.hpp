#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_MESSAGERECEIVED_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_MESSAGERECEIVED_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct MessageReceived: public tin::network::bsdsocket::Event
    {
        std::string message;

        MessageReceived(
            const std::string& message
        );

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_MESSAGERECEIVED_HPP */


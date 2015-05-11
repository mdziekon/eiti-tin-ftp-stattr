#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper { namespace events
{
    struct OutcomingMessage: public tin::network::bsdsocket::wrapper::Event
    {
        const std::string message;

        OutcomingMessage(const std::string& message);
        virtual void accept(tin::network::bsdsocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP */


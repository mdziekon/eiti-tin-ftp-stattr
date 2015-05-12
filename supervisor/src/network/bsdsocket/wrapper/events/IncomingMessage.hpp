#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_INCOMINGMESSAGE_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_INCOMINGMESSAGE_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper { namespace events
{
    struct IncomingMessage: public tin::network::bsdsocket::wrapper::Event
    {
        std::string ip;
        unsigned int port;
        std::string message;

        IncomingMessage(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message
        );
        virtual void accept(tin::network::bsdsocket::wrapper::ClientVisitor& i);
    };
}}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_INCOMINGMESSAGE_HPP */


#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper { namespace events
{
    struct OutcomingMessage: public tin::network::bsdsocket::wrapper::Event
    {
        const std::string ip;
        const unsigned int port;
        const std::string message;
        const bool waitForResponse;

        OutcomingMessage(const std::string& ip, const unsigned int& port, const std::string& message, const bool& waitForResponse);
        virtual void accept(tin::network::bsdsocket::wrapper::ClientVisitor& i);
    };
}}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_OUTCOMINGMESSAGE_HPP */


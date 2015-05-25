#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_ResponseReceived_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_ResponseReceived_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ResponseReceived: public tin::network::bsdsocket::Event
    {
        std::string ip;
        unsigned int port;
        std::string message;

        ResponseReceived(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message
        );

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_ResponseReceived_HPP */


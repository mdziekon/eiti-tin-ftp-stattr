#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct MessageRequest: public tin::network::bsdsocket::Event
    {
        std::string ip;
        unsigned int port;
        std::string message;
        bool waitForResponse;

        MessageRequest(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message,
            const bool& waitForResponse
        );

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP */


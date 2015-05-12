#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP

#include <string>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ResponseRequest: public tin::network::bsdsocket::Event
    {
        std::string message;

        ResponseRequest(
            const std::string& message
        );

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP */


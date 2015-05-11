#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP

#include <memory>

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ResponseRequest: public tin::network::bsdsocket::Event
    {
        ResponseRequest();

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP */


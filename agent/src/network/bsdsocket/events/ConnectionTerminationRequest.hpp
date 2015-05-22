#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_CONNECTIONTERMINATIONREQUEST_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_CONNECTIONTERMINATIONREQUEST_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ConnectionTerminationRequest: public tin::network::bsdsocket::Event
    {
        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_CONNECTIONTERMINATIONREQUEST_HPP */


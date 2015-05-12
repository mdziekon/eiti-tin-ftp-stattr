#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_TERMINATE_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_TERMINATE_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct Terminate: public tin::network::bsdsocket::Event
    {
        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_TERMINATENETWORKMANAGER_HPP */


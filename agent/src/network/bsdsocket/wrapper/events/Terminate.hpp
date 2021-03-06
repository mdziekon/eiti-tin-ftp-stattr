#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_TERMINATE_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_TERMINATE_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper { namespace events
{
    struct Terminate: public tin::network::bsdsocket::wrapper::Event
    {
        virtual void accept(tin::network::bsdsocket::wrapper::ServerVisitor& i);
    };
}}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENTS_TERMINATE_HPP */


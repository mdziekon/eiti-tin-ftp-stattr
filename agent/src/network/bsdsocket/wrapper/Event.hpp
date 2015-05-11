#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP

#include "../../../utils/Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class ServerVisitor;

    struct Event:
    public tin::utils::Event<tin::network::bsdsocket::wrapper::ServerVisitor>
    {
        virtual void accept(tin::network::bsdsocket::wrapper::ServerVisitor& i) = 0;
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP */


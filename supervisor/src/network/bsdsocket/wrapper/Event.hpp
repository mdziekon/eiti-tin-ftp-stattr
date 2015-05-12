#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP

#include "../../../utils/Event.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class ClientVisitor;

    struct Event:
    public tin::utils::Event<tin::network::bsdsocket::wrapper::ClientVisitor>
    {
        virtual void accept(tin::network::bsdsocket::wrapper::ClientVisitor& i) = 0;
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_EVENT_HPP */


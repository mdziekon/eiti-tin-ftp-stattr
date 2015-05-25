#ifndef TIN_NETWORK_BSDSOCKET_EVENT_HPP
#define TIN_NETWORK_BSDSOCKET_EVENT_HPP

#include "../../utils/Event.hpp"

namespace tin { namespace network { namespace bsdsocket
{
    class ManagerVisitor;

    struct Event: public tin::utils::Event<tin::network::bsdsocket::ManagerVisitor>
    {
        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_NETWORK_BSDSOCKET_EVENT_HPP */

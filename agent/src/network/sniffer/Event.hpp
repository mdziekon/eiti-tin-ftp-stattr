#ifndef TIN_NETWORK_SNIFFER_EVENT_HPP
#define TIN_NETWORK_SNIFFER_EVENT_HPP

#include "../../utils/Event.hpp"

namespace tin { namespace network { namespace sniffer
{
    class ManagerVisitor;

    struct Event: public tin::utils::Event<tin::network::sniffer::ManagerVisitor>
    {
        virtual void accept(tin::network::sniffer::ManagerVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_NETWORK_SNIFFER_EVENT_HPP */

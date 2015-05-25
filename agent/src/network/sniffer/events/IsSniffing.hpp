#ifndef TIN_NETWORK_SNIFFER_EVENTS_ISSNIFFING_HPP
#define TIN_NETWORK_SNIFFER_EVENTS_ISSNIFFING_HPP

#include "../Event.hpp"


namespace tin { namespace network { namespace sniffer { namespace events
{
    struct IsSniffing: public tin::network::sniffer::Event
    {
        virtual void accept(tin::network::sniffer::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_SNIFFER_EVENTS_ISSNIFFING_HPP */


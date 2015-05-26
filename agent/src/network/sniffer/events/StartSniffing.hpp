#ifndef TIN_NETWORK_SNIFFER_EVENTS_STARTSNIFFING_HPP
#define TIN_NETWORK_SNIFFER_EVENTS_STARTSNIFFING_HPP

#include "../Event.hpp"

namespace tin { namespace network { namespace sniffer { namespace events
{
    struct StartSniffing: public tin::network::sniffer::Event
    {
        virtual void accept(tin::network::sniffer::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_SNIFFER_EVENTS_STARTSNIFFING_HPP */
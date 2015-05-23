#ifndef TIN_NETWORK_SNIFFER_EVENTS_CHANGEFILTER_HPP
#define TIN_NETWORK_SNIFFER_EVENTS_CHANGEFILTER_HPP

#include "../Event.hpp"

#include <string>

namespace tin { namespace network { namespace sniffer { namespace events
{
    struct ChangeFilter: public tin::network::sniffer::Event
    {
        const std::string device;
        const std::string expression;

        ChangeFilter(const std::string& device, const std::string& expression);

        virtual void accept(tin::network::sniffer::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_SNIFFER_EVENTS_CHANGEFILTER_HPP */


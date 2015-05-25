#ifndef TIN_NETWORK_SNIFFER_TYPEDEFS_HPP
#define TIN_NETWORK_SNIFFER_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace network { namespace sniffer
{
    struct Event;

    typedef std::shared_ptr<tin::network::sniffer::Event> EventPtr;
    typedef Queue<EventPtr> ManagerQueue;
}}}

#endif  /* TIN_NETWORK_SNIFFER_TYPEDEFS_HPP */


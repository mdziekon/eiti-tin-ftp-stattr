#ifndef TIN_NETWORK_BSDSOCKET_TYPEDEFS_HPP
#define TIN_NETWORK_BSDSOCKET_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace network { namespace bsdsocket
{
    struct Event;

    typedef std::shared_ptr<tin::network::bsdsocket::Event> EventPtr;
    typedef Queue<EventPtr> ManagerQueue;
}}}

#endif  /* TIN_NETWORK_BSDSOCKET_TYPEDEFS_HPP */


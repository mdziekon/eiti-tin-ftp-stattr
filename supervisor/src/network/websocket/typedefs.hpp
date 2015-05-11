#ifndef TIN_NETWORK_WEBSOCKET_TYPEDEFS_HPP
#define	TIN_NETWORK_WEBSOCKET_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace network { namespace websocket
{
    struct Event;

    typedef std::shared_ptr<tin::network::websocket::Event> EventPtr;
    typedef Queue<EventPtr> ManagerQueue;
}}}

#endif	/* TIN_NETWORK_WEBSOCKET_TYPEDEFS_HPP */


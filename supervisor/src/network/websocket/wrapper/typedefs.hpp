#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_TYPEDEFS_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace network { namespace websocket { namespace wrapper
{
    struct Event;

    typedef std::shared_ptr<tin::network::websocket::wrapper::Event> EventPtr;
    typedef Queue<EventPtr> ServerQueue;
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_TYPEDEFS_HPP */


#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_TYPEDEFS_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    struct Event;

    typedef std::shared_ptr<tin::network::bsdsocket::wrapper::Event> EventPtr;
    typedef Queue<EventPtr> ServerQueue;
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_TYPEDEFS_HPP */


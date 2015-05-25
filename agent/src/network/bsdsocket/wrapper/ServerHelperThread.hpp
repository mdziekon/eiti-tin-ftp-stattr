#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP

#include "typedefs.hpp"
#include "../../../utils/Thread.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Server;

    class ServerHelperThread: public tin::utils::Thread
    {
    public:
        tin::network::bsdsocket::wrapper::Server& server;
        tin::network::bsdsocket::wrapper::ServerQueue& incomingQueue;

        ServerHelperThread(
            tin::network::bsdsocket::wrapper::Server& server,
            tin::network::bsdsocket::wrapper::ServerQueue& incomingQueue
        );
        virtual void threadTask();

    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP */


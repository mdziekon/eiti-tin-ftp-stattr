#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP

#include "typedefs.hpp"
#include "../../../utils/Thread.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper
{
    class Server;

    class ServerHelperThread: public tin::utils::Thread
    {
    public:
        tin::network::websocket::wrapper::Server& server;
        tin::network::websocket::wrapper::ServerQueue& incomingQueue;

        ServerHelperThread(
            tin::network::websocket::wrapper::Server& server,
            tin::network::websocket::wrapper::ServerQueue& incomingQueue
        );
        virtual void threadTask();

    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERHELPERTHREAD_HPP */


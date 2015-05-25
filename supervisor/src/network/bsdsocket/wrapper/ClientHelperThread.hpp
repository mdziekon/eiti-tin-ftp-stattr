#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTHELPERTHREAD_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTHELPERTHREAD_HPP

#include "typedefs.hpp"
#include "../../../utils/Thread.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Client;

    class ClientHelperThread: public tin::utils::Thread
    {
    public:
        tin::network::bsdsocket::wrapper::Client& client;
        tin::network::bsdsocket::wrapper::ClientQueue& incomingQueue;

        ClientHelperThread(
            tin::network::bsdsocket::wrapper::Client& client,
            tin::network::bsdsocket::wrapper::ClientQueue& incomingQueue
        );
        virtual void threadTask();

    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTHELPERTHREAD_HPP */


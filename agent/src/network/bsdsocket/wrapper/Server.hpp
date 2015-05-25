#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP

#include <mutex>
#include <condition_variable>

#include "typedefs.hpp"

#include "ServerHelperThread.hpp"
#include "ServerVisitor.hpp"
#include "../../../utils/HandlersContainer.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Server
    {
        friend class tin::network::bsdsocket::wrapper::ServerVisitor;
        friend class tin::network::bsdsocket::wrapper::ServerHelperThread;

    public:
        Server(const unsigned int& portNo);
        ~Server();
        void run();
        void terminate();

        unsigned int attachMessageReceivedHandler(
            std::function<void(const std::string&)>& handler
        );
        unsigned int attachMessageReceivedHandler(
            std::function<void(const std::string&)>&& handler
        );

        void sendResponse(
            const std::string& message
        );
        void terminateConnection();

    private:
        unsigned int portNo;

        int socketHandle;
        int connectionHandle;
        bool connectionActionAwaiting = false;

        std::mutex connectionMtx;
        std::unique_lock<std::mutex> connectionLock;
        std::condition_variable connectionLockCnd;

        tin::network::bsdsocket::wrapper::ServerQueue receiverQueue;
        tin::network::bsdsocket::wrapper::ServerQueue transmitterQueue;

        std::thread listenerThread;
        std::thread receiverThread;
        std::thread transmitterThread;
        tin::network::bsdsocket::wrapper::ServerHelperThread receiverHelperThread;
        tin::network::bsdsocket::wrapper::ServerHelperThread transmitterHelperThread;

        tin::network::bsdsocket::wrapper::ServerVisitor visitor;

        tin::utils::HandlersContainer<void(const std::string&)>
            messageReceivedHandlers;


        void runMessageReceivedHandlers(const std::string& message);

        void listenerLoop();
        void unlockConnection();

        void onMessageReceive(const std::string& message);
        void onResponseRequest(const std::string& message);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP */

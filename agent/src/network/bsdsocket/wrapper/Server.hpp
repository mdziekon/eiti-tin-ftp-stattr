#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP

#include "typedefs.hpp"

#include "ServerHelperThread.hpp"
#include "ServerVisitor.hpp"
#include "utils/HandlersContainer.hpp"

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
            std::function<void(const unsigned int&, const std::string&)>& handler
        );
        unsigned int attachMessageReceivedHandler(
            std::function<void(const unsigned int&, const std::string&)>&& handler
        );

        void sendMessage(
            const std::string& message
        );

    private:
        unsigned int portNo;

        tin::network::bsdsocket::wrapper::ServerQueue receiverQueue;
        tin::network::bsdsocket::wrapper::ServerQueue transmitterQueue;

        std::thread listenerThread;
        std::thread receiverThread;
        std::thread transmitterThread;
        tin::network::bsdsocket::wrapper::ServerHelperThread receiverHelperThread;
        tin::network::bsdsocket::wrapper::ServerHelperThread transmitterHelperThread;

        tin::network::bsdsocket::wrapper::ServerVisitor visitor;

        tin::network::bsdsocket::wrapper::utils::HandlersContainer<void(const std::string&)>
            messageReceivedHandlers;


        void runMessageReceivedHandlers(const std::string& message);

        void listenerLoop();

        void onMessageReceive();
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_SERVER_HPP */

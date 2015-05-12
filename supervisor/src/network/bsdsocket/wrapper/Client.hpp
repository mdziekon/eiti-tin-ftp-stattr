#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENT_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENT_HPP

#include <mutex>
#include <condition_variable>

#include "typedefs.hpp"

#include "ClientHelperThread.hpp"
#include "ClientVisitor.hpp"
#include "utils/HandlersContainer.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Client
    {
        friend class tin::network::bsdsocket::wrapper::ClientVisitor;
        friend class tin::network::bsdsocket::wrapper::ClientHelperThread;

    public:
        Client();
        ~Client();
        void run();
        void terminate();

        unsigned int attachResponseReceivedHandler(
            std::function<void(const std::string&, const unsigned int&, const std::string&)>& handler
        );
        unsigned int attachResponseReceivedHandler(
            std::function<void(const std::string&, const unsigned int&, const std::string&)>&& handler
        );

        void sendMessage(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message,
            const bool& waitForResponse
        );

    private:
        int socketHandle;

        tin::network::bsdsocket::wrapper::ClientQueue receiverQueue;
        tin::network::bsdsocket::wrapper::ClientQueue transmitterQueue;

        std::thread receiverThread;
        std::thread transmitterThread;
        tin::network::bsdsocket::wrapper::ClientHelperThread receiverHelperThread;
        tin::network::bsdsocket::wrapper::ClientHelperThread transmitterHelperThread;

        tin::network::bsdsocket::wrapper::ClientVisitor visitor;

        tin::network::bsdsocket::wrapper::utils::HandlersContainer
        <void(const std::string&, const unsigned int&, const std::string&)>
            messageReceivedHandlers;


        void runMessageReceivedHandlers(const std::string& ip, const unsigned int& port, const std::string& message);

        void onMessageRequest(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message,
            const bool& waitForResponse
        );
        void onResponseReceive(
            const std::string& ip,
            const unsigned int& port,
            const std::string& message
        );
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENT_HPP */

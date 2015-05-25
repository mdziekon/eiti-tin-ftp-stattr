#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_SERVER_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_SERVER_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <mutex>

#include "typedefs.hpp"

#include "ServerHelperThread.hpp"
#include "ServerVisitor.hpp"
#include "../../../utils/HandlersContainer.hpp"

namespace tin { namespace network { namespace websocket { namespace wrapper
{
    class Server
    {
        // TODO: sendMessage() method should probably return Future/Promise
        // enabling us to attach onSuccess/onError handlers

        // TODO: Handle & throw exceptions

        friend class tin::network::websocket::wrapper::ServerVisitor;
        friend class tin::network::websocket::wrapper::ServerHelperThread;

    public:
        Server(const unsigned int& portNo);
        ~Server();
        void run();
        void terminate();

        unsigned int attachConnectionOpenedHandler(
            std::function<void(const unsigned int&)>& handler
        );
        unsigned int attachConnectionClosedHandler(
            std::function<void(const unsigned int&)>& handler
        );
        unsigned int attachMessageReceivedHandler(
            std::function<void(const unsigned int&, const std::string&)>& handler
        );
        unsigned int attachConnectionOpenedHandler(
            std::function<void(const unsigned int&)>&& handler
        );
        unsigned int attachConnectionClosedHandler(
            std::function<void(const unsigned int&)>&& handler
        );
        unsigned int attachMessageReceivedHandler(
            std::function<void(const unsigned int&, const std::string&)>&& handler
        );

        void sendMessage(
            unsigned int& connectionID,
            const std::string& message,
            const websocketpp::frame::opcode::value& method = websocketpp::frame::opcode::text
        );
        void sendMessage(
            const std::vector<unsigned int>& connectionIDs,
            const std::string& message,
            const websocketpp::frame::opcode::value& method = websocketpp::frame::opcode::text
        );
        void broadcastMessage(
            const std::string& message,
            const websocketpp::frame::opcode::value& method = websocketpp::frame::opcode::text
        );

    private:
        websocketpp::server<websocketpp::config::asio> server;
        unsigned int portNo;

        tin::network::websocket::wrapper::ServerQueue receiverQueue;
        tin::network::websocket::wrapper::ServerQueue transmitterQueue;

        std::recursive_mutex accountingMtx;

        std::thread listenerThread;
        std::thread receiverThread;
        std::thread transmitterThread;
        tin::network::websocket::wrapper::ServerHelperThread receiverHelperThread;
        tin::network::websocket::wrapper::ServerHelperThread transmitterHelperThread;

        tin::network::websocket::wrapper::ServerVisitor visitor;

        std::unordered_map<unsigned int, websocketpp::connection_hdl> connections;
        std::map<
            websocketpp::connection_hdl,
            unsigned int,
            std::owner_less<websocketpp::connection_hdl>
        > connectionIDs;
        unsigned int nextConnectionID = 0;

        tin::utils::HandlersContainer<void(const unsigned int&)>
            connectionOpenedHandlers;
        tin::utils::HandlersContainer<void(const unsigned int&)>
            connectionClosedHandlers;
        tin::utils::HandlersContainer<void(const unsigned int&, const std::string&)>
            messageReceivedHandlers;


        unsigned int insertNewConnection(websocketpp::connection_hdl& connectionHandler);
        unsigned int removeConnection(websocketpp::connection_hdl& connectionHandler);
        unsigned int getConnectionID(websocketpp::connection_hdl& connectionHandler);

        void runConnectionOpenedHandlers(const unsigned int& idx);
        void runConnectionClosedHandlers(const unsigned int& idx);
        void runMessageReceivedHandlers(const unsigned int& idx, const std::string& message);

        void listenerLoop();

        void onConnectionOpen(websocketpp::connection_hdl connectionHandler);
        void onConnectionClose(websocketpp::connection_hdl connectionHandler);
        void onMessageReceive(
            websocketpp::connection_hdl connectionHandler,
            websocketpp::server<websocketpp::config::asio>::message_ptr message
        );

        websocketpp::connection_hdl& getConnectionHandler(
            const unsigned int& connectionID
        );
    };
}}}}

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_SERVER_HPP */

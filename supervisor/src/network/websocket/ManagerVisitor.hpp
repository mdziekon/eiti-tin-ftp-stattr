#ifndef TIN_NETWORK_WEBSOCKET_MANAGERVISITOR_HPP
#define	TIN_NETWORK_WEBSOCKET_MANAGERVISITOR_HPP

namespace tin { namespace network { namespace websocket
{
    class Manager;

    namespace events
    {
        struct TerminateNetworkManager;
        struct NewServerConnection;
        struct ServerConnectionClosed;
        struct MessageReceived;
        struct MessageSendRequest;
        struct MessageSendMultiRequest;
        struct MessageBroadcastRequest;
    }

    class ManagerVisitor
    {
        friend class tin::network::websocket::Manager;

    public:
        void visit(tin::network::websocket::events::TerminateNetworkManager& evt);
        void visit(tin::network::websocket::events::NewServerConnection& evt);
        void visit(tin::network::websocket::events::ServerConnectionClosed& evt);
        void visit(tin::network::websocket::events::MessageReceived& evt);
        void visit(tin::network::websocket::events::MessageSendRequest& evt);
        void visit(tin::network::websocket::events::MessageSendMultiRequest& evt);
        void visit(tin::network::websocket::events::MessageBroadcastRequest& evt);

    private:
        tin::network::websocket::Manager& manager;

        ManagerVisitor(tin::network::websocket::Manager& manager);
    };
}}}

#endif	/* TIN_NETWORK_WEBSOCKET_MANAGERVISITOR_HPP */


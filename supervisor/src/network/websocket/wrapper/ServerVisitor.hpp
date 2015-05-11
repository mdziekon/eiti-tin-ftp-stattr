#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERVISITOR_HPP
#define TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERVISITOR_HPP

namespace tin { namespace network { namespace websocket { namespace wrapper
{
    class Server;

    namespace events
    {
        struct Disconnection;
        struct Message;
        struct OutputMessage;
        struct OutputMultiMessage;
        struct BroadcastMessage;
        struct NewConnection;
        struct Terminate;
    }

    class ServerVisitor
    {
        friend class tin::network::websocket::wrapper::Server;

    private:
        tin::network::websocket::wrapper::Server& server;

        ServerVisitor(tin::network::websocket::wrapper::Server& server);

    public:
        void visit(tin::network::websocket::wrapper::events::Disconnection& e);
        void visit(tin::network::websocket::wrapper::events::Message& e);
        void visit(tin::network::websocket::wrapper::events::OutputMessage& e);
        void visit(tin::network::websocket::wrapper::events::OutputMultiMessage& e);
        void visit(tin::network::websocket::wrapper::events::BroadcastMessage& e);
        void visit(tin::network::websocket::wrapper::events::NewConnection& e);
        void visit(tin::network::websocket::wrapper::events::Terminate& e);
    };
}}}}

#endif /* TIN_NETWORK_WEBSOCKET_WRAPPER_SERVERVISITOR_HPP */

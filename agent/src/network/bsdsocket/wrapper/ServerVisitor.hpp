#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERVISITOR_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERVISITOR_HPP

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Server;

    namespace events
    {
        struct IncomingMessage;
        struct OutcomingMessage;
        struct Terminate;
    }

    class ServerVisitor
    {
        friend class tin::network::bsdsocket::wrapper::Server;

    private:
        tin::network::bsdsocket::wrapper::Server& server;

        ServerVisitor(tin::network::bsdsocket::wrapper::Server& server);

    public:
        void visit(tin::network::bsdsocket::wrapper::events::IncomingMessage& e);
        void visit(tin::network::bsdsocket::wrapper::events::OutcomingMessage& e);
        void visit(tin::network::bsdsocket::wrapper::events::Terminate& e);
    };
}}}}

#endif /* TIN_NETWORK_BSDSOCKET_WRAPPER_SERVERVISITOR_HPP */

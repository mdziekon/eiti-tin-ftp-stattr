#ifndef TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTVISITOR_HPP
#define TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTVISITOR_HPP

namespace tin { namespace network { namespace bsdsocket { namespace wrapper
{
    class Client;

    namespace events
    {
        struct IncomingMessage;
        struct OutcomingMessage;
        struct Terminate;
    }

    class ClientVisitor
    {
        friend class tin::network::bsdsocket::wrapper::Client;

    private:
        tin::network::bsdsocket::wrapper::Client& client;

        ClientVisitor(tin::network::bsdsocket::wrapper::Client& client);

    public:
        void visit(tin::network::bsdsocket::wrapper::events::IncomingMessage& e);
        void visit(tin::network::bsdsocket::wrapper::events::OutcomingMessage& e);
        void visit(tin::network::bsdsocket::wrapper::events::Terminate& e);
    };
}}}}

#endif /* TIN_NETWORK_BSDSOCKET_WRAPPER_CLIENTVISITOR_HPP */

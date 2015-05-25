#ifndef TIN_NETWORK_BSDSOCKET_MANAGERVISITOR_HPP
#define TIN_NETWORK_BSDSOCKET_MANAGERVISITOR_HPP

namespace tin { namespace network { namespace bsdsocket
{
    class Manager;

    namespace events
    {
        struct Terminate;
        struct MessageReceived;
        struct ResponseRequest;
    }

    class ManagerVisitor
    {
        friend class tin::network::bsdsocket::Manager;

    public:
        void visit(tin::network::bsdsocket::events::Terminate& evt);
        void visit(tin::network::bsdsocket::events::MessageReceived& evt);
        void visit(tin::network::bsdsocket::events::ResponseRequest& evt);

    private:
        tin::network::bsdsocket::Manager& manager;

        ManagerVisitor(tin::network::bsdsocket::Manager& manager);
    };
}}}

#endif  /* TIN_NETWORK_BSDSOCKET_MANAGERVISITOR_HPP */


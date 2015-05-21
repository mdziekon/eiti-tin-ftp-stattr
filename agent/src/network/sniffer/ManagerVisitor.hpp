#ifndef TIN_NETWORK_SNIFFER_MANAGERVISITOR_HPP
#define TIN_NETWORK_SNIFFER_MANAGERVISITOR_HPP

namespace tin { namespace network { namespace sniffer
{
    class Manager;

    namespace events
    {
        struct Terminate;
        struct ChangeFilter;
    }

    class ManagerVisitor
    {
        friend class tin::network::sniffer::Manager;

    public:
        void visit(tin::network::sniffer::events::Terminate& evt);
        void visit(tin::network::sniffer::events::ChangeFilter& evt);

    private:
        tin::network::sniffer::Manager& manager;

        ManagerVisitor(tin::network::sniffer::Manager& manager);
    };
}}}

#endif  /* TIN_NETWORK_SNIFFER_MANAGERVISITOR_HPP */


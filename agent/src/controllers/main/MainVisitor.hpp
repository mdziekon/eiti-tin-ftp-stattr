#ifndef TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP
#define	TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP

namespace tin { namespace controllers { namespace main
{
    class MainModule;

    namespace events
    {
        struct Terminate;
        struct CmdReceived;
        struct PacketReceived;
        struct NetworkReply;
        struct PacketsRequestReceived;
    }

    class MainVisitor
    {
        friend class tin::controllers::main::MainModule;

        tin::controllers::main::MainModule& controller;

        MainVisitor(tin::controllers::main::MainModule& controller);

    public:
        void visit(tin::controllers::main::events::Terminate &event);
        void visit(tin::controllers::main::events::CmdReceived &event);
        void visit(tin::controllers::main::events::PacketReceived &event);
        void visit(tin::controllers::main::events::NetworkReply &event);
        void visit(tin::controllers::main::events::PacketsRequestReceived &event);
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP */


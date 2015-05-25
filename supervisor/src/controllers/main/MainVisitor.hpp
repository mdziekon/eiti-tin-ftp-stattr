#ifndef TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP
#define	TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP

namespace tin { namespace controllers { namespace main
{
    class MainModule;

    namespace events
    {
        struct Terminate;
        struct CmdResponseReceived;
        struct WebClientRequestReceived;
    }

    class MainVisitor
    {
        friend class tin::controllers::main::MainModule;

        tin::controllers::main::MainModule& controller;

        MainVisitor(tin::controllers::main::MainModule& controller);

    public:
        void visit(tin::controllers::main::events::Terminate &evt);
        void visit(tin::controllers::main::events::CmdResponseReceived &evt);
        void visit(tin::controllers::main::events::WebClientRequestReceived &evt);
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP */


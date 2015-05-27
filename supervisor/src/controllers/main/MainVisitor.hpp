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
        struct NetworkRequest;
        struct WebClientSendRequest;
    }

    class MainVisitor
    {
        friend class tin::controllers::main::MainModule;

        tin::controllers::main::MainModule& controller;

        void resendEvent(events::WebClientRequestReceived &evt);
        
        MainVisitor(tin::controllers::main::MainModule& controller);

    public:
        void visit(tin::controllers::main::events::Terminate &evt);
        void visit(tin::controllers::main::events::CmdResponseReceived &evt);
        void visit(tin::controllers::main::events::WebClientRequestReceived &evt);
        void visit(tin::controllers::main::events::NetworkRequest &evt);
        void visit(tin::controllers::main::events::WebClientSendRequest &evt);
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP */


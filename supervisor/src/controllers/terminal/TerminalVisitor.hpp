#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALVISITOR_HPP
#define	TIN_CONTROLLERS_TERMINAL_TERMINALVISITOR_HPP

namespace tin { namespace controllers { namespace terminal
{
    class TerminalModule;

    namespace events
    {
        struct Terminate;
    }

    class TerminalVisitor
    {
        friend class tin::controllers::terminal::TerminalModule;

        tin::controllers::terminal::TerminalModule& controller;

        TerminalVisitor(tin::controllers::terminal::TerminalModule& controller);

    public:
        void visit(tin::controllers::terminal::events::Terminate &evt);
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALVISITOR_HPP */


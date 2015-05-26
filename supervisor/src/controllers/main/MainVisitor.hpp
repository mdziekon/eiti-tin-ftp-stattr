#ifndef TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP
#define	TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP

#include <tuple>
#include "../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main
{
    class MainModule;

    namespace events
    {
        struct Terminate;
        struct CmdResponseReceived;
        struct JSONRequestReceived;
        struct NetworkRequest;
    }

    class MainVisitor
    {
        friend class tin::controllers::main::MainModule;

        tin::controllers::main::MainModule& controller;

        MainVisitor(tin::controllers::main::MainModule& controller);

    public:
        void visit(tin::controllers::main::events::Terminate &evt);
        void visit(tin::controllers::main::events::CmdResponseReceived &evt);
        void visit(tin::controllers::main::events::JSONRequestReceived &evt);
        void visit(tin::controllers::main::events::NetworkRequest &evt);

    private:
        void resendEvent(events::JSONRequestReceived &evt);
        void resendDelayed(std::tuple<bool, unsigned int, tin::utils::json::ptr> obj);
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINVISITOR_HPP */


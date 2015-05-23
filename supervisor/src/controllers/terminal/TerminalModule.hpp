#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP

#include <vector>

#include "typedefs.hpp"
#include "../main/typedefs.hpp"
#include "../../utils/typedefs.hpp"

#include "../../utils/QueueThread.hpp"

#include "Event.hpp"
#include "TerminalVisitor.hpp"

namespace tin { namespace controllers { namespace terminal
{
    class TerminalModule: public tin::utils::QueueThread
    <tin::controllers::terminal::Event, tin::controllers::terminal::TerminalVisitor>
    {
        friend class tin::controllers::terminal::TerminalVisitor;

    public:
        TerminalModule(
    		tin::controllers::terminal::TerminalQueue &incomingQueue,
            tin::controllers::main::ControllerQueue &controllerQueue
        );
    private:
        tin::controllers::main::ControllerQueue& controllerQueue;
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP */

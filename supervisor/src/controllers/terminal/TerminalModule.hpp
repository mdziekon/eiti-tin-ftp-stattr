#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP

#include <vector>
#include <boost/asio.hpp>

#include "typedefs.hpp"
#include "../main/typedefs.hpp"
#include "../../utils/typedefs.hpp"

#include "../../utils/QueueThread.hpp"

#include "Event.hpp"
#include "TerminalVisitor.hpp"
#include "TerminalServer.hpp"

using boost::asio::ip::tcp;


namespace tin { namespace controllers { namespace terminal
{
    class TerminalModule: public tin::utils::QueueThread
    <tin::controllers::terminal::Event, tin::controllers::terminal::TerminalVisitor>
    {
        friend class tin::controllers::terminal::TerminalVisitor;

    public:
        TerminalModule(
    		tin::controllers::terminal::TerminalQueue &incomingQueue,
            tin::controllers::main::ControllerQueue &controllerQueue,
            boost::asio::io_service& io_service
        );
    private:
        tin::controllers::main::ControllerQueue& controllerQueue;
        tin::controllers::terminal::TerminalServer* terminalServer_;
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP */

#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP

#include <vector>

#include "typedefs.hpp"
#include "../main/typedefs.hpp"
#include "../../utils/typedefs.hpp"

#include "../../utils/QueueThread.hpp"
#include <boost/shared_ptr.hpp>
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

        void send_message(const char* message);
    private:
        std::shared_ptr<tin::controllers::terminal::TerminalServer> terminalServerPtr_;
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALMODULE_HPP */

#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALSERVER_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALSERVER_HPP

#include <boost/asio.hpp>
#include "../main/typedefs.hpp"

using boost::asio::ip::tcp;

namespace tin { namespace controllers { namespace terminal
{
    class TerminalServer
    {
    public:
        TerminalServer(
            tin::controllers::main::ControllerQueue &controllerQueue,
            boost::asio::io_service& io_service,
        	short port);
    private:
        tin::controllers::main::ControllerQueue& controllerQueue;
    	void do_accept();
    	tcp::acceptor acceptor_;
    	tcp::socket socket_;
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALSERVER_HPP */

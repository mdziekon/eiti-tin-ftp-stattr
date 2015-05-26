#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP

#include <boost/asio.hpp>
#include "../main/typedefs.hpp"
#include "../../../../terminal/src/terminal_message.hpp"

using boost::asio::ip::tcp;

namespace tin { namespace controllers { namespace terminal
{
    class TerminalSession :
    	public std::enable_shared_from_this <TerminalSession>
    {
    public:
        TerminalSession(
            tin::controllers::main::ControllerQueue &controllerQueue,
            tcp::socket socket);
        void start();
    private:
        tin::controllers::main::ControllerQueue& controllerQueue;
    	void do_read_header();
        void do_read_body();
    	void do_write(const char* message);
    	tcp::socket socket_;
        terminal_message msg_;
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP */

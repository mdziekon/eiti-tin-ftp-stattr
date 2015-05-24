#ifndef TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP
#define TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP

#include <boost/asio.hpp>
#include "../../../terminal/src/command_message.hpp"

using boost::asio::ip::tcp;

namespace tin { namespace controllers { namespace terminal
{
    class TerminalSession :
    	public std::enable_shared_from_this <TerminalSession>
    {
    public:
        TerminalSession(tcp::socket socket);
        void start();
    private:
    	void do_read();
    	void do_write(int length);

    	tcp::socket socket_;
    	enum { max_length = 1024 };
    	char data_[max_length];
    };
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TERMINALSESSION_HPP */

#ifndef TIN_CONTROLLERS_TERMINAL_TYPEDEFS_HPP
#define	TIN_CONTROLLERS_TERMINAL_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace controllers { namespace terminal
{
    struct Event;

    typedef std::shared_ptr<tin::controllers::terminal::Event> EventPtr;
    typedef Queue<EventPtr> TerminalQueue;
}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_TYPEDEFS_HPP */


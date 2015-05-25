#ifndef TIN_CONTROLLERS_TERMINAL_EVENTS_TERMINATE_HPP
#define TIN_CONTROLLERS_TERMINAL_EVENTS_TERMINATE_HPP

#include "../Event.hpp"

namespace tin { namespace controllers { namespace terminal { namespace events
{
    struct Terminate: public tin::controllers::terminal::Event
    {
        virtual void accept(tin::controllers::terminal::TerminalVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_TERMINAL_EVENTS_TERMINATE_HPP */


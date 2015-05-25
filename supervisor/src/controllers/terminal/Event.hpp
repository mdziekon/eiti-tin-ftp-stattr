#ifndef TIN_CONTROLLERS_TERMINAL_EVENT_HPP
#define TIN_CONTROLLERS_TERMINAL_EVENT_HPP

#include "../../utils/Event.hpp"

namespace tin { namespace controllers { namespace terminal
{
    class TerminalVisitor;

    struct Event: public tin::utils::Event<tin::controllers::terminal::TerminalVisitor>
    {
        virtual void accept(tin::controllers::terminal::TerminalVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_CONTROLLERS_TERMINAL_EVENT_HPP */

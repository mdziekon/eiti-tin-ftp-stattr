#ifndef TIN_CONTROLLERS_MAIN_EVENTS_TERMINATE_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_TERMINATE_HPP

#include "../Event.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct Terminate: public tin::controllers::main::Event
    {
        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_TERMINATE_HPP */


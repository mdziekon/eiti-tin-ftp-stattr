#ifndef TIN_CONTROLLERS_MAIN_EVENT_HPP
#define TIN_CONTROLLERS_MAIN_EVENT_HPP

#include "../../utils/Event.hpp"

namespace tin { namespace controllers { namespace main
{
    class MainVisitor;

    struct Event: public tin::utils::Event<tin::controllers::main::MainVisitor>
    {
        virtual void accept(tin::controllers::main::MainVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_CONTROLLERS_MAIN_EVENT_HPP */

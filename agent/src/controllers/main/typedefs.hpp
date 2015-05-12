#ifndef TIN_CONTROLLERS_MAIN_TYPEDEFS_HPP
#define	TIN_CONTROLLERS_MAIN_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

namespace tin { namespace controllers { namespace main
{
    struct Event;

    typedef std::shared_ptr<tin::controllers::main::Event> EventPtr;
    typedef Queue<tin::controllers::main::EventPtr> ControllerQueue;
}}}

#endif	/* TIN_CONTROLLERS_MAIN_TYPEDEFS_HPP */


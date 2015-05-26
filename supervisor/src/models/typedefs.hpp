#ifndef TIN_SUPERVISOR_SRC_MODELS_TYPEDEFS_HPP
#define TIN_SUPERVISOR_SRC_MODELS_TYPEDEFS_HPP

#include <memory>
#include <concurrent-queue/Queue.hpp>

#include "Event.hpp"

namespace tin { namespace supervisor { namespace models 
{
    typedef std::shared_ptr<tin::supervisor::models::Event> EventPtr;
    typedef Queue<EventPtr> StatsQueue;
}}}

#endif
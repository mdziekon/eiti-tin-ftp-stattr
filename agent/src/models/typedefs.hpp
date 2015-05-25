#ifndef TIN_AGENT_SRC_MODELS_TYPEDEFS_HPP
#define TIN_AGENT_SRC_MODELS_TYPEDEFS_HPP

#include <concurrent-queue/Queue.hpp>
#include <memory>

namespace tin { namespace agent { namespace models
{
    struct Event;
    
    typedef std::shared_ptr<tin::agent::models::Event> EventPtr;
    typedef Queue<tin::agent::models::EventPtr> StatsGathererQueue;
}}}

#endif
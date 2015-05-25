#ifndef TIN_AGENT_SRC_MODELS_EVENTS_REQESTPACKETS_HPP
#define TIN_AGENT_SRC_MODELS_EVENTS_REQESTPACKETS_HPP

#include <sys/types.h>
#include "../Event.hpp"

namespace tin { namespace agent { namespace models
{
    class StatsGathererVisitor;
}}}

namespace tin { namespace agent { namespace models { namespace events
{
    struct RequestPackets: public tin::agent::models::Event
    {       
        void accept(tin::agent::models::StatsGathererVisitor& visitor);
    };
}}}}

#endif
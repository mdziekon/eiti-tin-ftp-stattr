#ifndef TIN_AGENT_SRC_MODELS_EVENT_HPP
#define TIN_AGENT_SRC_MODELS_EVENT_HPP

#include "../utils/Event.hpp"

namespace tin { namespace agent { namespace models
{
    class StatsGathererVisitor;

    struct Event: public tin::utils::Event<tin::agent::models::StatsGathererVisitor>
    {
        virtual void accept(tin::agent::models::StatsGathererVisitor& visitor) = 0;
    };
}}}

#endif /* TIN_AGENT_SRC_MODELS_EVENT_HPP */
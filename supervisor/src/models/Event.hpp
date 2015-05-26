#ifndef TIN_SUPERVISOR_SRC_MODELS_EVENT_HPP
#define TIN_SUPERVISOR_SRC_MODELS_EVENT_HPP

#include "../utils/Event.hpp"
#include "StatsVisitor.hpp"

namespace tin { namespace supervisor { namespace models
{
    struct Event: public tin::utils::Event<tin::supervisor::models::StatsVisitor>
    {
        virtual void accept(tin::supervisor::models::StatsVisitor& visitor) = 0;
    };
}}}

#endif
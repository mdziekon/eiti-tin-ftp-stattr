#ifndef TIN_SUPERVISOR_SRC_MODELS_STATS_HPP
#define TIN_SUPERVISOR_SRC_MODELS_STATS_HPP

#include <json/src/json.hpp>
#include <sys/types.h>

#include "../utils/QueueThread.hpp"
#include "../controllers/main/typedefs.hpp"
#include "typedefs.hpp"

namespace tin { namespace supervisor { namespace models
{
    class Stats: public tin::utils::QueueThread<
        tin::supervisor::models::Event, 
        tin::supervisor::models::StatsVisitor
    >
    {
        friend class tin::supervisor::models::StatsVisitor;
        
        tin::controllers::main::ControllerQueue& controllerQueue;
        
    public:
        void updateDataset();
        
        Stats(
            tin::supervisor::models::StatsQueue& statsQueue,
            tin::controllers::main::ControllerQueue& controllerQueue
        );
    };
}}}
#endif
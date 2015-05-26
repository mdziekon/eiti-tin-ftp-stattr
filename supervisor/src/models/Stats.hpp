#ifndef TIN_SUPERVISOR_SRC_MODELS_STATS_HPP
#define TIN_SUPERVISOR_SRC_MODELS_STATS_HPP

#include <map>
#include <json/src/json.hpp>
#include <thread>
#include <sys/types.h>

#include "../utils/QueueThread.hpp"
#include "../controllers/main/typedefs.hpp"
#include "typedefs.hpp"
#include "../utils/JSON.hpp"

namespace tin { namespace supervisor { namespace models
{
    class Stats: public tin::utils::QueueThread<
        tin::supervisor::models::Event, 
        tin::supervisor::models::StatsVisitor
    >
    {
        friend class tin::supervisor::models::StatsVisitor;
        
        tin::controllers::main::ControllerQueue& controllerQueue;
        
        std::map<int64_t, tin::utils::json::ptr> packetsByTimestamp;
        std::map<u_int32_t, tin::utils::json::ptr> packetsBySourceIP;
        std::map<u_int32_t, tin::utils::json::ptr> packetsByDestinationIP;

        bool requestorsActive = true;
    public:
        void updateDataset();
        const tin::utils::json::ptr computeStatsPerDay(const tin::utils::json::ptr& requestorData) const;
        const tin::utils::json::ptr computeIndividualUsage(const tin::utils::json::ptr& requestorData) const;

        std::thread createRequestorThread(
            const u_int32_t& intervalMilliseconds,
            tin::controllers::main::ControllerQueue& controlerQueue
        );

        Stats(
            tin::supervisor::models::StatsQueue& statsQueue,
            tin::controllers::main::ControllerQueue& controllerQueue
        );
        ~Stats();
    };
}}}
#endif
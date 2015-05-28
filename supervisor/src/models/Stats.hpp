#ifndef TIN_SUPERVISOR_SRC_MODELS_STATS_HPP
#define TIN_SUPERVISOR_SRC_MODELS_STATS_HPP

#include <map>
#include <json/src/json.hpp>
#include <thread>
#include <sys/types.h>

#include <fstream>

#include "../utils/QueueThread.hpp"
#include "../controllers/main/typedefs.hpp"
#include "typedefs.hpp"
#include "../utils/JSON.hpp"
#include "../utils/Packet.hpp"
#include "../models/MachinesStorage.hpp"

namespace tin { namespace supervisor { namespace models
{
    class Stats: public tin::utils::QueueThread<
        tin::supervisor::models::Event,
        tin::supervisor::models::StatsVisitor
    >
    {
        friend class tin::supervisor::models::StatsVisitor;

        tin::controllers::main::ControllerQueue& controllerQueue;

        tin::models::MachinesStorage& machines;
        std::vector<tin::utils::Packet> packets;

        std::fstream packetsFile;
        std::string lastPacketsFile;

        void packetsFileOpen(const std::string &filename);
        void packetsFileLoadData();
        void packetsFileAppendData(nlohmann::json &packetsArray);

        void appendPackets(nlohmann::json &packetsArray);

    public:
        const tin::utils::json::ptr computeStatsPerDay(const tin::utils::json::ptr& requestorData) const;
        const tin::utils::json::ptr computeIndividualUsage(const tin::utils::json::ptr& requestorData) const;
        const tin::utils::json::ptr computeMachinesPerDay(const tin::utils::json::ptr& requestorData, int machineID) const;
        const tin::utils::json::ptr computeMachinesPerConnection(const tin::utils::json::ptr& requestorData, int machineID) const;
        const tin::utils::json::ptr computeMachinesPerHour(const tin::utils::json::ptr& requestorData, int machineID) const;

        std::thread createRequestorThread(
            const u_int32_t& intervalMilliseconds,
            tin::controllers::main::ControllerQueue& controlerQueue
        );

        Stats(
            tin::supervisor::models::StatsQueue& statsQueue,
            tin::controllers::main::ControllerQueue& controllerQueue,
            tin::models::MachinesStorage& machines
        );
    };
}}}
#endif

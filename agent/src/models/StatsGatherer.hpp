#ifndef TIN_AGENT_SRC_MODELS_STATSGATHERER_HPP
#define TIN_AGENT_SRC_MODELS_STATSGATHERER_HPP

#include <sys/types.h>
#include <vector>

#include "typedefs.hpp"
#include "../controllers/main/typedefs.hpp"
#include "../utils/typedefs.hpp"

#include "Event.hpp"
#include "StatsGathererVisitor.hpp"
#include "../utils/QueueThread.hpp"
#include "../utils/Packet.hpp"


namespace tin { namespace agent { namespace models
{
    class StatsGatherer: public tin::utils::QueueThread<
        tin::agent::models::Event,
        tin::agent::models::StatsGathererVisitor
    >
    {
        friend class tin::agent::models::StatsGathererVisitor;
        
        tin::controllers::main::ControllerQueue& controllerQueue;
        std::vector<tin::utils::Packet::ptr> capturedPackets;
        
        void incomingPacket(const tin::utils::Packet::ptr& packet);
        void fetchPackets();
    public:

        StatsGatherer(
            tin::agent::models::StatsGathererQueue& queue,
            tin::controllers::main::ControllerQueue& controllerQueue
        );
    };
}}}

#endif
#ifndef TIN_AGENT_SRC_MODELS_INCOMINGPACKETEVENT_HPP
#define TIN_AGENT_SRC_MODELS_INCOMINGPACKETEVENT_HPP

#include "../Event.hpp"
#include "../../utils/Packet.hpp"
#include "../../utils/typedefs.hpp"

namespace tin { namespace agent { namespace models { namespace events
{
    struct IncomingPacket : public tin::agent::models::Event
    {
        
        const tin::utils::Packet::ptr packet;
        
        void accept(tin::agent::models::StatsGathererVisitor& visitor) override;
        
        IncomingPacket(const tin::utils::Packet::ptr& packet);
    };
}}}}

#endif
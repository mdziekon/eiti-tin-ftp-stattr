#ifndef TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP
#define TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP

#include <vector>

#include "../Event.hpp"
#include "../../utils/JSON.hpp"

namespace tin { namespace supervisor { namespace models { namespace events
{
    struct ReceivePackets: public tin::supervisor::models::Event
    {
        std::vector<tin::utils::json::ptr> packets;
        
        virtual void accept(tin::supervisor::models::StatsVisitor& visitor);
        
        ReceivePackets(const std::vector<tin::utils::json::ptr>& packets) = delete;
        ReceivePackets(const std::vector<tin::utils::json::ptr>&& packets);
    };
}}}}

#endif  /* TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP */
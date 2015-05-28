#ifndef TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP
#define TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP

#include <vector>

#include "../Event.hpp"
#include "../../utils/JSON.hpp"

namespace tin { namespace supervisor { namespace models { namespace events
{
    struct ReceivePackets: public tin::supervisor::models::Event
    {
        tin::utils::json::ptr packetsArray;

        virtual void accept(tin::supervisor::models::StatsVisitor& visitor);

        ReceivePackets(const tin::utils::json::ptr &packetsArray);
    };
}}}}

#endif  /* TIN_SUPERVISOR_SRC_EVENTS_RECEIVEPACKETS_HPP */

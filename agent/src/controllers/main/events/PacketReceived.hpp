#ifndef TIN_CONTROLLERS_MAIN_EVENTS_PACKETRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_PACKETRECEIVED_HPP

#include "../Event.hpp"
#include "../../../utils/packet.h"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct PacketReceived: public tin::controllers::main::Event
    {
        const tin::utils::packet::ptr pac;

        PacketReceived(const tin::utils::packet::ptr& packet);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP */
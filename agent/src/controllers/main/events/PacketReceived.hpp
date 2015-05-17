#ifndef TIN_CONTROLLERS_MAIN_EVENTS_PACKETRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_PACKETRECEIVED_HPP

#include "../Event.hpp"
#include "../../../utils/Packet.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct PacketReceived: public tin::controllers::main::Event
    {
        const tin::utils::Packet::ptr pac;

        PacketReceived(const tin::utils::Packet::ptr& packet);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP */

#ifndef TIN_CONTROLLERS_MAIN_EVENTS_SNIFFERSTATUS_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_SNIFFERSTATUS_HPP

#include "../Event.hpp"
#include "../../../utils/Packet.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct SnifferStatus: public tin::controllers::main::Event
    {
        bool is;

        SnifferStatus(const bool is);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_SNIFFERSTATUS_HPP */

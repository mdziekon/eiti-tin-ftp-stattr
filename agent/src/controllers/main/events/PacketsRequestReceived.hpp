#ifndef TIN_CONTROLLERS_MAIN_EVENTS_PACKETSREQUESTRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_PACKETSREQUESTRECEIVED_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct PacketsRequestReceived: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;

        PacketsRequestReceived(const tin::utils::json::ptr& jsonPtr);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif
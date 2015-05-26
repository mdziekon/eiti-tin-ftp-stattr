#ifndef TIN_CONTROLLERS_MAIN_EVENTS_JSONRequestReceived_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_JSONRequestReceived_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct JSONRequestReceived: public tin::controllers::main::Event
    {
        const bool fromTerminal;
        const tin::utils::json::ptr jsonPtr;
        unsigned int connectionID;

        JSONRequestReceived(bool fromTerminal, const tin::utils::json::ptr& jsonPtr, unsigned int connectionID);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_JSONRequestReceived_HPP */

#ifndef TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct CmdReceived: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;

        CmdReceived(const tin::utils::json::ptr& jsonPtr);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP */


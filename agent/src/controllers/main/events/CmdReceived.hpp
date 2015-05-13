#ifndef TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP

#include <memory>

#include <json/src/json.hpp>

#include "../Event.hpp"

using json = nlohmann::json;

namespace tin { namespace controllers { namespace main { namespace events
{
    struct CmdReceived: public tin::controllers::main::Event
    {
        const std::shared_ptr<json> jsonPtr;

        CmdReceived(const std::shared_ptr<json>& jsonPtr);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_CMDRECEIVED_HPP */


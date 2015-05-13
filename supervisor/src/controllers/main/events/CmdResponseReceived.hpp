#ifndef TIN_CONTROLLERS_MAIN_EVENTS_CMDRESPONSERECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_CMDRESPONSERECEIVED_HPP

#include <string>
#include <memory>

#include <json/src/json.hpp>

#include "../Event.hpp"

using json = nlohmann::json;

namespace tin { namespace controllers { namespace main { namespace events
{
    struct CmdResponseReceived: public tin::controllers::main::Event
    {
        const std::string ip;
        const unsigned int port;
        const std::shared_ptr<json> jsonPtr;

        CmdResponseReceived(
            const std::string& ip,
            const unsigned int& port,
            const std::shared_ptr<json>& jsonPtr
        );

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_CMDRESPONSERECEIVED_HPP */


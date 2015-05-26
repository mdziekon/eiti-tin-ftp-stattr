#ifndef TIN_AGENT_SRC_CONTROLLERS_MAIN_NETWORKREPLY_HPP
#define TIN_AGENT_SRC_CONTROLLERS_MAIN_NETWORKREPLY_HPP

#include <json/src/json.hpp>

#include "../Event.hpp"

namespace tin { namespace controllers { namespace main
{
    class MainVisitor;
}}}


namespace tin { namespace controllers { namespace main { namespace events
{
    struct NetworkReply: tin::controllers::main::Event
    {
        const nlohmann::json reply;
        
        void accept(tin::controllers::main::MainVisitor& visitor);
        
        NetworkReply(const nlohmann::json& reply);
    };
}}}}

#endif
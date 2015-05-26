#ifndef TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETREQUESTRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETREQUESTRECEIVED_HPP

#include <sys/types.h>

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct WebsocketRequestReceived: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;

        WebsocketRequestReceived(
            const tin::utils::json::ptr& jsonPtr
        );

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif  /* TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETREQUESTRECEIVED_HPP */
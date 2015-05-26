#ifndef TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETBROADCASTREQUEST_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETBROADCASTREQUEST_HPP

#include <sys/types.h>

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct WebsocketBroadcastRequest: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;

        WebsocketBroadcastRequest(
            const tin::utils::json::ptr& jsonPtr
        );

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif  /* TIN_CONTROLLERS_MAIN_EVENTS_WEBSOCKETBROADCASTREQUEST_HPP */
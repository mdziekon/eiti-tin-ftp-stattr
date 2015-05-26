#ifndef TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTSENDREQUEST_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTSENDREQUEST_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct WebClientSendRequest: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;
        unsigned int connectionID;

        WebClientSendRequest(const tin::utils::json::ptr& jsonPtr, unsigned int connectionID);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif  /* TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTSENDREQUEST_HPP */
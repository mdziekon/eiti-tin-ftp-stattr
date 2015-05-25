#ifndef TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTREQUESTRECEIVED_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTREQUESTRECEIVED_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct WebClientRequestReceived: public tin::controllers::main::Event
    {
        const tin::utils::json::ptr jsonPtr;

        WebClientRequestReceived(const tin::utils::json::ptr& jsonPtr);

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif	/* TIN_CONTROLLERS_MAIN_EVENTS_WEBCLIENTREQUESTRECEIVED_HPP */
#ifndef TIN_CONTROLLERS_MAIN_EVENTS_NETWORKREQUEST_HPP
#define TIN_CONTROLLERS_MAIN_EVENTS_NETWORKREQUEST_HPP

#include <sys/types.h>

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace main { namespace events
{
    struct NetworkRequest: public tin::controllers::main::Event
    {
        const std::string ip;
        const u_int32_t port;
        const tin::utils::json::ptr jsonPtr;

        NetworkRequest(
            const std::string& ip,
            const u_int32_t& port,
            const tin::utils::json::ptr& jsonPtr
        );

        virtual void accept(tin::controllers::main::MainVisitor& visitor);
    };
}}}}

#endif  /* TIN_CONTROLLERS_MAIN_EVENTS_NETWORKREQUEST_HPP */
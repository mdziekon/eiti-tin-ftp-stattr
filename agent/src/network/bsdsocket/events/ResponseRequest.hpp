#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ResponseRequest: public tin::network::bsdsocket::Event
    {
        const tin::utils::json::ptr jsonPtr;

        ResponseRequest(const tin::utils::json::ptr& jsonPtr);

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP */


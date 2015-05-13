#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP

#include <memory>

#include <json/src/json.hpp>

#include "../Event.hpp"

using json = nlohmann::json;

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct ResponseRequest: public tin::network::bsdsocket::Event
    {
        const std::shared_ptr<json> jsonPtr;

        ResponseRequest(const std::shared_ptr<json>& jsonPtr);

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_RESPONSEREQUEST_HPP */


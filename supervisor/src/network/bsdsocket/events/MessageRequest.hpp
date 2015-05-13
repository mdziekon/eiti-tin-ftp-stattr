#ifndef TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP
#define TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP

#include <memory>

#include <json/src/json.hpp>

#include "../Event.hpp"

using json = nlohmann::json;

namespace tin { namespace network { namespace bsdsocket { namespace events
{
    struct MessageRequest: public tin::network::bsdsocket::Event
    {
        std::string ip;
        unsigned int port;
        const std::shared_ptr<json> jsonPtr;
        bool waitForResponse;

        MessageRequest(
            const std::string& ip,
            const unsigned int& port,
            const std::shared_ptr<json>& jsonPtr,
            const bool& waitForResponse
        );

        virtual void accept(tin::network::bsdsocket::ManagerVisitor& visitor);
    };
}}}}

#endif  /* TIN_NETWORK_BSDSOCKET_EVENTS_MessageRequest_HPP */


#include "WebsocketBroadcastRequest.hpp"

#include "../MainVisitor.hpp"

using namespace tin::controllers::main::events;

void WebsocketBroadcastRequest::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

WebsocketBroadcastRequest::WebsocketBroadcastRequest(const tin::utils::json::ptr& jsonPtr):
jsonPtr(jsonPtr)
{}

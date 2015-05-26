#include "WebsocketRequestReceived.hpp"

#include "../MainVisitor.hpp"

using namespace tin::controllers::main::events;

void WebsocketRequestReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

WebsocketRequestReceived::WebsocketRequestReceived(const tin::utils::json::ptr& jsonPtr):
jsonPtr(jsonPtr)
{}

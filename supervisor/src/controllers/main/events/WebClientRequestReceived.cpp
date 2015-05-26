#include "WebClientRequestReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::WebClientRequestReceived;

WebClientRequestReceived::WebClientRequestReceived(const tin::utils::json::ptr& jsonPtr, unsigned int connectionID):
jsonPtr(jsonPtr), connectionID(connectionID)
{}

void WebClientRequestReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

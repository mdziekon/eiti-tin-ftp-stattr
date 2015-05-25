#include "WebClientRequestReceived.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::WebClientRequestReceived;

WebClientRequestReceived::WebClientRequestReceived(const tin::utils::json::ptr& jsonPtr):
jsonPtr(jsonPtr)
{}

void WebClientRequestReceived::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

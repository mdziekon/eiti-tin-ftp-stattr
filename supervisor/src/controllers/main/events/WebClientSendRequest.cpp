#include "WebClientSendRequest.hpp"

#include "../MainVisitor.hpp"

void tin::controllers::main::events::WebClientSendRequest::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

tin::controllers::main::events::WebClientSendRequest::WebClientSendRequest(
    const tin::utils::json::ptr& jsonPtr, 
    unsigned int connectionID
):
jsonPtr(jsonPtr),
connectionID(connectionID)
{}

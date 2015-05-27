#include "RequestAnalytics.hpp"

#include "../StatsVisitor.hpp"

using namespace tin::supervisor::models::events;

void RequestAnalytics::accept(tin::supervisor::models::StatsVisitor& visitor)
{
    visitor.visit(*this);
}

RequestAnalytics::RequestAnalytics(
    const u_int32_t& connectionID,
    const tin::utils::json::ptr& requestData
):
connectionID(connectionID),
requestData(requestData)
{}
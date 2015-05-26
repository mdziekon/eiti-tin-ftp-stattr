#include "RequestAnalytics.hpp"

#include "../StatsVisitor.hpp"

using namespace tin::supervisor::models::events;

void RequestAnalytics::accept(tin::supervisor::models::StatsVisitor& visitor)
{
    visitor.visit(*this);
}

RequestAnalytics::RequestAnalytics(
    const std::string& ip,
    const u_int32_t& port,
    const tin::utils::json::ptr& requestData
):
ip(ip),
port(port),
requestData(requestData)
{}
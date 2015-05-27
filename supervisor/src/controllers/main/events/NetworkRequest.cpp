#include "NetworkRequest.hpp"

#include "../MainVisitor.hpp"

using namespace tin::controllers::main::events;

void NetworkRequest::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

NetworkRequest::NetworkRequest(
    const std::string& ip,
    const u_int32_t& port,
    const tin::utils::json::ptr& jsonPtr
):
ip(ip),
port(port),
jsonPtr(jsonPtr)
{}

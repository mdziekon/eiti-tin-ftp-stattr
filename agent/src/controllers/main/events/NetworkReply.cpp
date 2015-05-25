#include "NetworkReply.hpp"

#include "../MainVisitor.hpp"

using namespace tin::controllers::main::events;

void NetworkReply::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}

NetworkReply::NetworkReply(const nlohmann::json& reply):
reply(reply)
{}
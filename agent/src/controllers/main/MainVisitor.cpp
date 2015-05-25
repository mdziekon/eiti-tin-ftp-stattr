#include "MainVisitor.hpp"

#include <iostream>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdReceived.hpp"
#include "events/PacketReceived.hpp"
#include "events/NetworkReply.hpp"
#include "../../models/events/RequestPackets.hpp"
#include "../../models/events/IncomingPacket.hpp"

#include "../../network/bsdsocket/events/ResponseRequest.hpp"

namespace events = tin::controllers::main::events;
namespace bsdsocketEvents = tin::network::bsdsocket::events;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &event)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdReceived &event)
{
    if(event.jsonPtr->find("action") != (*event.jsonPtr).end() &&
       event.jsonPtr->find("requestor_id") != (*event.jsonPtr).end()
    ) {
        if((*event.jsonPtr)["action"].get<std::string>() == "fetch_packets") {
            this->controller.statsGathererQueue.push(
                std::make_shared<tin::agent::models::events::RequestPackets>((*event.jsonPtr)["requestor_id"].get<u_int32_t>())
            );
        }
    }
}

void tin::controllers::main::MainVisitor::visit(events::PacketReceived& event)
{
    this->controller.statsGathererQueue.push(std::make_shared<tin::agent::models::events::IncomingPacket>(event.pac));
}

void tin::controllers::main::MainVisitor::visit(events::NetworkReply& event)
{
    this->controller.networkManagerQueue.push(
        std::make_shared<tin::network::bsdsocket::events::ResponseRequest>(
            std::make_shared<nlohmann::json>(event.reply)
        )
    );
}


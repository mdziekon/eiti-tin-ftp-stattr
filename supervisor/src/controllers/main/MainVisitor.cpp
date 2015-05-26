#include "MainVisitor.hpp"

#include <iostream>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdResponseReceived.hpp"
#include "events/NetworkRequest.hpp"
#include "events/WebsocketBroadcastRequest.hpp"

#include "../../network/websocket/typedefs.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"

#include "../../network/bsdsocket/events/MessageRequest.hpp"

#include "../../models/events/RequestAnalytics.hpp"

namespace events = tin::controllers::main::events;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &evt)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdResponseReceived &evt)
{
    std::cout << "[Supervisor] [MainCtrl] Received response: " << evt.jsonPtr->dump() << std::endl;
    std::cout << "                        Source: " << evt.ip << ":" << evt.port << std::endl;
    
    if(evt.jsonPtr->find("route") != evt.jsonPtr->end() &&
       evt.jsonPtr->find("type") != evt.jsonPtr->end() &&
       evt.jsonPtr->find("uid") != evt.jsonPtr->end()) {
        this->controller.statsQueue.push(std::make_shared<tin::supervisor::models::events::RequestAnalytics>(evt.ip, evt.port, evt.jsonPtr));
    }
}

void tin::controllers::main::MainVisitor::visit(events::NetworkRequest& evt)
{
    this->controller.bsdManagerQueue.push(
        std::make_shared<tin::network::bsdsocket::events::MessageRequest>(
            evt.ip,
            evt.port,
            evt.jsonPtr,
            false
        )
    );
}

void tin::controllers::main::MainVisitor::visit(events::WebsocketBroadcastRequest& evt)
{
    this->controller.networkManagerQueue.push(
        std::make_shared<tin::network::websocket::events::MessageBroadcastRequest>(
            evt.jsonPtr->dump()
        )
    );
}
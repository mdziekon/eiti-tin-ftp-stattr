#include "MainVisitor.hpp"

#include <iostream>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdResponseReceived.hpp"
#include "events/NetworkRequest.hpp"

#include "../../network/websocket/typedefs.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"

#include "../../network/bsdsocket/events/MessageRequest.hpp"

#include "../terminal/events/SendMessage.hpp"
#include "../../utils/JSON.hpp"

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
    
    this->controller.terminalQueue.push(
    	std::make_shared<tin::controllers::terminal::events::SendMessage>(
    		tin::utils::json::makeSharedInstance("{ \"Message\" : \"OK\" }")));
}

void tin::controllers::main::MainVisitor::visit(events::NetworkRequest& evt)
{
    this->controller.bsdManagerQueue.push(std::make_shared<tin::network::bsdsocket::events::MessageRequest>(evt.ip, evt.port, evt.jsonPtr, true));
}
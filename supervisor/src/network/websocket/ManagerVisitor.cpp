#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/TerminateNetworkManager.hpp"
#include "events/NewServerConnection.hpp"
#include "events/ServerConnectionClosed.hpp"
#include "events/MessageReceived.hpp"
#include "events/MessageSendRequest.hpp"
#include "events/MessageSendMultiRequest.hpp"
#include "events/MessageBroadcastRequest.hpp"

using tin::network::websocket::ManagerVisitor;
namespace events = tin::network::websocket::events;

#include <iostream>

void ManagerVisitor::visit(events::TerminateNetworkManager& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::NewServerConnection& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::ServerConnectionClosed& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::MessageReceived& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::MessageSendRequest& evt)
{
    this->manager.server.sendMessage(
        evt.serverConnectionID,
        "empty"
    );
}

void ManagerVisitor::visit(events::MessageSendMultiRequest& evt)
{
    this->manager.server.sendMessage(
        evt.serverConnectionIDs,
        "empty"
    );
}

void ManagerVisitor::visit(events::MessageBroadcastRequest& evt)
{
    this->manager.server.broadcastMessage(
        "empty"
    );
}

ManagerVisitor::ManagerVisitor(tin::network::websocket::Manager& manager):
manager(manager)
{}

#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/TerminateNetworkManager.hpp"
#include "events/NewServerConnection.hpp"
#include "events/ServerConnectionClosed.hpp"
#include "events/MessageReceived.hpp"
#include "events/MessageSendRequest.hpp"
#include "events/MessageSendMultiRequest.hpp"
#include "events/MessageBroadcastRequest.hpp"

#include "../../utils/JSON.hpp"

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
    auto jsonObj = nlohmann::json::parse(evt.message);

    if (!jsonObj["cmd"].is_string()) {
        jsonObj["error"] = {{ "invalid", "cmd" }};

        this->manager.server.sendMessage(evt.serverConnectionID, jsonObj.dump());
        return;
    }

    if (jsonObj["cmd"] == std::string("list_machines")) {
        jsonObj["data"] = {
            { "machines", {
                {
                    { "id", 1 },
                    { "name", "example machine" },
                    { "ip", "196.168.0.1" },
                    { "status", "sniffing" },
                    { "lastSync", 1432331838 }
                },
                {
                    { "id", 3 },
                    { "name", "test server #1" },
                    { "ip", "196.168.0.11" },
                    { "status", "stand-by" },
                    { "lastSync", 1432331838 }
                },
                {
                    { "id", 7 },
                    { "name", "test server #4" },
                    { "ip", "196.168.0.50" },
                    { "status", "offline" },
                    { "lastSync", 1432331838 }
                }
            }}
        };
    }

    this->manager.server.sendMessage(evt.serverConnectionID, jsonObj.dump());
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

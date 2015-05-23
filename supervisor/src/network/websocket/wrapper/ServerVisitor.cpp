#include "ServerVisitor.hpp"

#include "Server.hpp"

#include "events/Disconnection.hpp"
#include "events/Message.hpp"
#include "events/NewConnection.hpp"
#include "events/Terminate.hpp"
#include "events/OutputMessage.hpp"
#include "events/OutputMultiMessage.hpp"
#include "events/BroadcastMessage.hpp"

using tin::network::websocket::wrapper::ServerVisitor;
namespace events = tin::network::websocket::wrapper::events;

ServerVisitor::ServerVisitor(tin::network::websocket::wrapper::Server& server):
server(server)
{}

void ServerVisitor::visit(events::Disconnection& e)
{
    auto connectionID = this->server.removeConnection(e.connectionHandler);
    this->server.runConnectionClosedHandlers(connectionID);
}

void ServerVisitor::visit(events::Message& e)
{
    auto connectionID = this->server.getConnectionID(e.connectionHandler);
    std::string msgString = e.message->get_payload();
    this->server.runMessageReceivedHandlers(connectionID, msgString);
}

void ServerVisitor::visit(events::OutputMessage& e)
{
    auto connectionHandler = this->server.getConnectionHandler(e.connectionID);

    this->server.server.send(connectionHandler, e.message, e.method);
}

void ServerVisitor::visit(events::OutputMultiMessage& e)
{
    for(auto& connectionID: e.connectionIDs)
    {
        auto connectionHandler = this->server.getConnectionHandler(connectionID);

        this->server.server.send(connectionHandler, e.message, e.method);
    }
}

void ServerVisitor::visit(events::BroadcastMessage& e)
{
    for(auto& connection: this->server.connections)
    {
        this->server.server.send(connection.second, e.message, e.method);
    }
}

void ServerVisitor::visit(events::NewConnection& e)
{
    auto connectionID = this->server.insertNewConnection(e.connectionHandler);
    this->server.runConnectionOpenedHandlers(connectionID);
}

void ServerVisitor::visit(events::Terminate& e)
{}


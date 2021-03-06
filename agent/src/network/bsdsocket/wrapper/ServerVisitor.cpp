#include "ServerVisitor.hpp"

#include "Server.hpp"

#include "events/IncomingMessage.hpp"
#include "events/OutcomingMessage.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::wrapper::ServerVisitor;
namespace events = tin::network::bsdsocket::wrapper::events;

ServerVisitor::ServerVisitor(tin::network::bsdsocket::wrapper::Server& server):
server(server)
{}

void ServerVisitor::visit(events::IncomingMessage& e)
{
    this->server.runMessageReceivedHandlers(e.message);
}

void ServerVisitor::visit(events::OutcomingMessage& e)
{
    this->server.onResponseRequest(e.message);
}

void ServerVisitor::visit(events::Terminate& e)
{}


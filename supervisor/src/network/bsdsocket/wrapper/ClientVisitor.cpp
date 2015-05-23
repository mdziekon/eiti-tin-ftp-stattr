#include "ClientVisitor.hpp"

#include "Client.hpp"

#include "events/IncomingMessage.hpp"
#include "events/OutcomingMessage.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::wrapper::ClientVisitor;
namespace events = tin::network::bsdsocket::wrapper::events;

ClientVisitor::ClientVisitor(tin::network::bsdsocket::wrapper::Client& client):
client(client)
{}

void ClientVisitor::visit(events::IncomingMessage& e)
{
    this->client.runMessageReceivedHandlers(e.ip, e.port, e.message);
}

void ClientVisitor::visit(events::OutcomingMessage& e)
{
    this->client.onMessageRequest(e.ip, e.port, e.message, e.waitForResponse);
}

void ClientVisitor::visit(events::Terminate& e)
{}


#include "Message.hpp"

#include "../ServerVisitor.hpp"

using tin::network::websocket::wrapper::events::Message;

Message::Message(
    websocketpp::connection_hdl& connectionHandler,
    websocketpp::server<websocketpp::config::asio>::message_ptr& message
):
connectionHandler(connectionHandler),
message(message)
{}

void Message::accept(tin::network::websocket::wrapper::ServerVisitor& i)
{
    i.visit(*this);
}

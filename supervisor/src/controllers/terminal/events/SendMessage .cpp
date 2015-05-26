#include "SendMessage.hpp"

#include "../TerminalVisitor.hpp"

using tin::controllers::terminal::events::SendMessage;

SendMessage::SendMessage(
    const tin::utils::json::ptr& jsonPtr
):
jsonPtr(jsonPtr)
{}

void SendMessage::accept(tin::controllers::terminal::TerminalVisitor& visitor)
{
    visitor.visit(*this);
}

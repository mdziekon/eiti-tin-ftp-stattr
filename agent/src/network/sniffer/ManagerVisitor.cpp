#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/Terminate.hpp"
#include "events/ChangeFilter.hpp"

#include <iostream>

using tin::network::sniffer::ManagerVisitor;
namespace events = tin::network::sniffer::events;

void ManagerVisitor::visit(events::Terminate& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::ChangeFilter& evt)
{
    std::cout << "[Sniffer] Changing config" << std::endl;

    this->manager.sniffer.stopSniffing();
    this->manager.sniffer.changeConfig(evt.device, evt.expression);
    this->manager.sniffer.run();
}

ManagerVisitor::ManagerVisitor(tin::network::sniffer::Manager& manager):
manager(manager)
{}

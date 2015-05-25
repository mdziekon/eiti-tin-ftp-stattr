#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/Terminate.hpp"
#include "events/ChangeFilter.hpp"
#include "events/StopSniffing.hpp"
#include "events/StartSniffing.hpp"
#include "../../controllers/main/events/SnifferStatus.hpp"


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

void ManagerVisitor::visit(events::StopSniffing& evt)
{
    std::cout << "[Sniffer] Stop sniffing" << std::endl;

    this->manager.sniffer.stopSniffing();
}

void ManagerVisitor::visit(events::StartSniffing& evt)
{
    std::cout << "[Sniffer] Start sniffing" << std::endl;

    if(!this->manager.sniffer.isSniffing())
    	this->manager.sniffer.run();
}

void ManagerVisitor::visit(events::IsSniffing& evt)
{
    std::cout << "[Sniffer] IsSniffing" << std::endl;

    this->manager.controllerQueue.push
    (tin::controllers::main::EventPtr(new tin::controllers::main::events::SnifferStatus
    	(this->manager.sniffer.isSniffing())));
}


ManagerVisitor::ManagerVisitor(tin::network::sniffer::Manager& manager):
manager(manager)
{}

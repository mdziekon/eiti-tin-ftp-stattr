#include "MainVisitor.hpp"

#include <iostream>
#include <string.h>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdReceived.hpp"

#include "../../network/bsdsocket/events/ResponseRequest.hpp"
#include "../../network/sniffer/events/ChangeFilter.hpp"

namespace events = tin::controllers::main::events;
namespace bsdsocketEvents = tin::network::bsdsocket::events;
namespace snifferEvents = tin::network::sniffer::events;

using nlohmann::json;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &event)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdReceived &event)
{
	json& temp = *(event.jsonPtr);
	if(temp.find("cmd") != temp.end())
	{
		const std::string tmp = temp["cmd"];
		if (strcmp(tmp.c_str(), "sync") == 0) {
				this->controller.networkManagerQueue.push(
        		tin::network::bsdsocket::EventPtr(
            	new bsdsocketEvents::ResponseRequest(
                	tin::utils::json::makeSharedInstance("{ \"testMessage\": \"testResponse\", \"testTable\": { \"testArray\": [ 1, 2, 3 ], \"test\": true } }")
            	)));

    	} else if(strcmp(tmp.c_str(),"change filter") == 0) {
    		if((temp.find("device") != temp.end()) && (temp.find("expression") != temp.end())) {
				this->controller.snifferManagerQueue.push(
				tin::network::sniffer::EventPtr(
					new snifferEvents::ChangeFilter(temp["device"], temp["expression"])
				));
			}

		}

	
	}
    
}

void tin::controllers::main::MainVisitor::visit(tin::controllers::main::events::PacketReceived &event)
{

}


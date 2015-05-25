#include "MainVisitor.hpp"

#include <iostream>
#include <chrono>
#include "Event.hpp"
#include "typedefs.hpp"

#include "MainModule.hpp"


#include "events/Terminate.hpp"
#include "events/CmdResponseReceived.hpp"
#include "../../utils/Machine.hpp"

#include "../../network/websocket/typedefs.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"

namespace events = tin::controllers::main::events;
namespace main = tin::controllers::main;
using nlohmann::json;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &evt)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdResponseReceived &evt)
{
    std::cout << "[Supervisor] [MainCtrl] Received response: " << evt.jsonPtr->dump() << std::endl;
    std::cout << "                        Source: " << evt.ip << ":" << evt.port << std::endl;
    

    auto it = std::make_pair(evt.ip, evt.port);
    std::map<std::pair<std::string, unsigned int>, int>::iterator ipPortIdMapIt;
    std::map<int, utils::Machine>::iterator idMachineMapIt;
    ipPortIdMapIt = controller.ipPortIdMap.find(it);
    idMachineMapIt = controller.idMachineMap.find(ipPortIdMapIt->second);
    utils::Machine& m = idMachineMapIt->second;

    json& temp = *(evt.jsonPtr);

    if (temp.find("error") != temp.end() && temp["error"].is_string())
    {
        if(temp.find("notConnected") != temp.end())
        {
            bool error = temp["error"]["notConnected"]; 
            if(error == true)
    	       m.status = "offline";
        }
        std::cout << "[Supervisor][MainCtrl] Error received" << std::endl;
        return;
    }

    if(temp.find("cmd") != temp.end() && temp["cmd"].is_string())
    {
        const std::string cmd = temp["cmd"];

        if (cmd == "sync")
        {
            auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch());
            m.lastSynchronization = ms.count();
        }

        else if (cmd == "ping")
        {
            if(temp.find("response") != temp.end())
            {
                const std::string response = temp["error"]["response"]; 
                if(response == "sniffing")
                    m.status("sniffing");
                if(response == "stand-by")
                    m.status.assign("stand-by");
            }
            
        }

        else if (cmd == "change_filter")
        {
        	const std::string expr = temp["expression"];
        	m.filter = expr;
        	m.status = "sniffing";
        }
    }

}

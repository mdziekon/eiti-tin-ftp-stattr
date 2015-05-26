#include "MainVisitor.hpp"

#include <iostream>
#include <chrono>
#include "Event.hpp"
#include "typedefs.hpp"

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdResponseReceived.hpp"
#include "events/WebClientRequestReceived.hpp"
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
                    m.status.assign("sniffing");
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

void tin::controllers::main::MainVisitor::visit(events::WebClientRequestReceived &evt)
{
    std::cout << "[Supervisor] WebClient Request Received, processing" << std::endl;

    json& temp = *(evt.jsonPtr);

    if(temp.find("route") == temp.end() || temp.find("type") == temp.end() ||
        !temp["route"].is_string() || !temp["type"].is_string())
        {
            std::cout << "[Supervisor] Bad WebClient request" << std::endl;
            return;
        }

    std::string route = temp["route"];
    std::string type = temp["type"];

    if (route == "machines" && type == "GET")
    {
        json jsonObj;
        jsonObj["data"] = {
                { "machines", {} }
            };
            
            for(auto& it: this->controller.idMachineMap)
            {
                jsonObj["data"]["machines"][jsonObj["data"]["machines"].size()] = {
                    { "id", it.second.id},
                    { "name", it.second.name },
                    { "ip", it.second.ip },
                    { "port", it.second.port },
                    { "status", it.second.status },
                    { "lastSync", it.second.lastSynchronization }
                    };
            }
    }


    if(route == "machines" && type == "POST")
    {
        if(temp.find("data") == temp.end())
        {
            std::cout << "[Supervisor] No data in POST" << std::endl;
            return;
        }

        const std::string ip = temp["data"]["ip"];
        const std::string name = temp["data"]["name"];
        unsigned port = temp["data"]["port"];

        utils::Machine m = utils::Machine(ip, name, port);

        this->controller.ipPortIdMap.insert(
            std::pair<std::pair<std::string, unsigned int>, int>
             (std::make_pair(ip, port), m.id));
        this->controller.idMachineMap.insert(std::pair<int, utils::Machine>(m.id, m));

    }


    if (route.substr(0, 8) == "machine/")
        {
            auto routeRest = route.substr(8);
            auto action = std::string("");
            if (routeRest.find("/") != std::string::npos)
            {
                action = routeRest.substr(routeRest.find("/") + 1);
                routeRest = routeRest.substr(0, routeRest.find("/"));
            }

            try
            {
               int machineID = static_cast<int>(std::stoul(routeRest));

               utils::Machine& machine = this->controller.idMachineMap.at(machineID);

                if (action == "" && type == "GET")
                {
                    temp["data"] = {
                        { "id", machineID },
                        { "name", machine.name },
                        { "ip", machine.ip },
                        { "port", machine.port },
                        { "status", machine.status },
                        { "lastSync", machine.lastSynchronization }};
                    
                }
                else if (action == "" && type == "PATCH")
                {

                    std::string name = temp["data"]["name"];
                    std::string ip = temp["data"]["ip"];
                    unsigned int port = temp["data"]["port"];

                    
                    machine.name = name;
                    machine.ip = ip;
                    machine.port = port;

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "" && type == "DELETE")
                {
                    
                    temp["data"] = {{ "success", true }};
                }
                else if (action == "sync" && type == "POST")
                {
                    auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    );

                    machine.lastSynchronization = ms.count();

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "toggle-sniffer" && type == "POST")
                {
                    
                    if (machine.status == "sniffing")
                    {
                        machine.status = "stand-by";
                        temp["data"] = {{ "success", true }};
                    }
                    else if (machine.status == "stand-by")
                    {
                        machine.status = "sniffing";
                        temp["data"] = {{ "success", true }};
                    }
                    else
                    {
                        temp["error"] = {{ "invalid", { {"status", machine.status} } }};
                    } 
                }
                else
                {
                    temp["error"] = {{ "invalid", { {"action", action} } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                temp["error"] = {{ "invalid", { {"routeID", true} } }};
            }
        }


}
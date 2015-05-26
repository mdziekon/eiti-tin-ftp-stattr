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
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"

namespace events = tin::controllers::main::events;
namespace main = tin::controllers::main;
namespace websocket = tin::network::websocket;
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

    auto& machine = this->controller.machines.getMachine(evt.ip, evt.port);

    json& temp = *(evt.jsonPtr);

    if(temp["cmd"].is_string())
    {
        const std::string cmd = temp["cmd"];

        if (cmd == "sync")
        {
            auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch());
            machine.lastSynchronization = ms.count();
        }
        else if (cmd == "ping")
        {
            if (temp["error"].is_object() && temp["error"]["notConnected"].is_boolean() && temp["error"]["notConnected"])
            {
                machine.status = "offline";
            }
            else if (temp["response"].is_string() && temp["response"] == std::string("pong"))
            {
                machine.status = "stand-by";
            }
        }

        else if (cmd == "change_filter")
        {
        	const std::string expr = temp["expression"];
        	machine.filter = expr;
        	machine.status = "sniffing";
        }
    }
    else
    {
        std::cout << "Invalid response received" << std::endl;
    }

}

void tin::controllers::main::MainVisitor::visit(events::WebClientRequestReceived &evt)
{
    std::cout << "[Supervisor] WebClient Request Received, processing" << std::endl;

    json& temp = *(evt.jsonPtr);

    try
    {
        if(temp.find("route") == temp.end() || temp.find("type") == temp.end() || !temp["route"].is_string() || !temp["type"].is_string())
        {
            temp["error"] = {{ "invalid", { {"route", "type"} } }};

            std::cout << "[Supervisor] Bad WebClient request" << std::endl;

            this->resendEvent(evt);
            return;
        }

        std::string route = temp["route"];
        std::string type = temp["type"];

        if (route == "machine" && type == "GET")
        {
            temp["data"] = {
                { "machines", json::array() }
            };

            for(auto& it: this->controller.machines.idMachineMap)
            {
                temp["data"]["machines"][temp["data"]["machines"].size()] = {
                    { "id", it.second.id},
                    { "name", it.second.name },
                    { "ip", it.second.ip },
                    { "port", it.second.port },
                    { "status", it.second.status },
                    { "lastSync", it.second.lastSynchronization }
                };
            }
        }
        else if(route == "machine" && type == "POST")
        {
            if(!temp["data"].is_object() || !temp["data"]["ip"].is_string() || !temp["data"]["name"].is_string() || !temp["data"]["port"].is_number())
            {
                std::cout << "[Supervisor] No data in POST" << std::endl;

                temp["error"] = {{ "invalid", { {"data", true} } }};

                this->resendEvent(evt);
                return;
            }

            const std::string ip = temp["data"]["ip"];
            const std::string name = temp["data"]["name"];
            unsigned port = temp["data"]["port"];

            this->controller.machines.newMachine(name, ip, port);

            temp["data"] = {{ "success", true }};
        }
        else if (route.substr(0, 8) == "machine/")
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

                auto& machine = this->controller.machines.getMachine(machineID);

                if (action == "" && type == "GET")
                {
                    temp["data"] = {
                        { "id", machineID },
                        { "name", machine.name },
                        { "ip", machine.ip },
                        { "port", machine.port },
                        { "status", machine.status },
                        { "lastSync", machine.lastSynchronization }
                    };
                }
                else if (action == "" && type == "PATCH")
                {
                    if(!temp["data"].is_object() || !temp["data"]["ip"].is_string() || !temp["data"]["name"].is_string() || !temp["data"]["port"].is_number())
                    {
                        std::cout << "[Supervisor] No data in PATCH" << std::endl;

                        temp["error"] = {{ "invalid", { {"data", true} } }};

                        this->resendEvent(evt);
                        return;
                    }

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
                    this->controller.machines.removeMachine(machineID);

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "sync" && type == "POST")
                {
                    // Do synchronization

                    auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    );

                    machine.lastSynchronization = ms.count();

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "toggle-sniffer" && type == "POST")
                {
                    // Toggle sniffer

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
                    temp["error"] = {{ "invalid", { { "action", action } } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                temp["error"] = {{ "invalid", { {"routeID", true} } }};
            }
        }
    }
    catch (std::exception& e)
    {
        temp["error"] = {{ "unknown", true }};
    }

    this->resendEvent(evt);
}

void tin::controllers::main::MainVisitor::resendEvent(events::WebClientRequestReceived &evt)
{
    this->controller.networkManagerQueue.push(
        std::make_shared<websocket::events::MessageSendRequest>(evt.connectionID, evt.jsonPtr)
    );
}

#include "MainVisitor.hpp"

#include <iostream>
#include <chrono>
#include "Event.hpp"
#include "typedefs.hpp"

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdResponseReceived.hpp"
#include "events/JSONRequestReceived.hpp"
#include "events/NetworkRequest.hpp"
#include "../../utils/Machine.hpp"

#include "../../network/websocket/typedefs.hpp"
#include "../../network/websocket/events/MessageSendRequest.hpp"
#include "../../network/websocket/events/MessageBroadcastRequest.hpp"
#include "../../network/bsdsocket/events/MessageRequest.hpp"

#include "../../network/bsdsocket/events/MessageRequest.hpp"

#include "../terminal/events/SendMessage.hpp"
#include "../../utils/JSON.hpp"

namespace events = tin::controllers::main::events;
namespace main = tin::controllers::main;
namespace websocket = tin::network::websocket;
namespace bsdsocket = tin::network::bsdsocket;
namespace terminal = tin::controllers::terminal;
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
    std::cout << "[Supervisor] [MainCtrl] Received response: " << evt.jsonPtr->dump().substr(0, 150) << std::endl;
    std::cout << "                        Source: " << evt.ip << ":" << evt.port << std::endl;

    auto& machine = this->controller.machines.getMachine(evt.ip, evt.port);

    json& temp = *(evt.jsonPtr);

    if(temp["cmd"].is_string())
    {
        const std::string cmd = temp["cmd"];

        if (cmd == "sync")
        {
            std::cout << "[Sync] " << "Received " << temp["data"].size() << " packets" << std::endl;

            auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch());
            machine.lastSynchronization = ms.count();

            auto queueIt = this->controller.syncQueue.find(std::pair<std::string, unsigned int>(evt.ip, evt.port));

            if (queueIt != this->controller.syncQueue.end())
            {
                auto& tempResp = *(std::get<2>(queueIt->second));
                tempResp["data"] = {{ "success", true }};

                this->resendDelayed(queueIt->second);

                this->controller.syncQueue.erase(queueIt);
            }
        }
        else if (cmd == "ping")
        {
            auto queueIt = this->controller.pingsQueue.find(std::pair<std::string, unsigned int>(evt.ip, evt.port));

            if (temp["error"].is_object() && temp["error"]["notConnected"].is_boolean() && temp["error"]["notConnected"])
            {
                machine.status = "offline";
            }
            else if (temp["response"].is_string())
            {
                std::string tt = temp["response"];
                machine.status = tt;
            }

            if (queueIt != this->controller.pingsQueue.end())
            {
                auto& temp = *(std::get<2>(queueIt->second));
                temp["data"] = {{ "success", true }};

                this->resendDelayed(queueIt->second);

                this->controller.pingsQueue.erase(queueIt);
            }
        }
        else if (cmd == "startsniffing" || cmd == "stopsniffing")
        {
            auto queueIt = this->controller.snifferToggleQueue.find(std::pair<std::string, unsigned int>(evt.ip, evt.port));
            bool success = false;

            if (temp["error"].is_object() && temp["error"]["notConnected"].is_boolean() && temp["error"]["notConnected"])
            {
                machine.status = "offline";
            }
            else if (temp["success"].is_boolean())
            {
                machine.status = (cmd == "startsniffing") ? "sniffing" : "stand-by";
                success = true;
            }

            if (queueIt != this->controller.pingsQueue.end())
            {
                auto& tempJSON = *(std::get<2>(queueIt->second));
                if (success)
                {
                    tempJSON["data"] = {{ "success", true }};
                }
                else
                {
                    tempJSON["error"] = {{ "success", false }};
                }

                this->resendDelayed(queueIt->second);

                this->controller.snifferToggleQueue.erase(queueIt);
            }
        }
        else if (cmd == "change_filter")
        {
            auto queueIt = this->controller.filterChangeQueue.find(std::pair<std::string, unsigned int>(evt.ip, evt.port));

            std::string t = temp["data"]["status"];
        	machine.status = t;

            if (queueIt != this->controller.pingsQueue.end())
            {
                auto& tempJSON = *(std::get<2>(queueIt->second));

                if (temp["success"])
                {
                    if (tempJSON["data"]["filterDevice"].is_string())
                    {
                        std::string tmp = tempJSON["data"]["filterDevice"];
                        machine.filterDevice = tmp;
                    }
                    if (tempJSON["data"]["filterExpression"].is_string())
                    {
                        std::string tmp = tempJSON["data"]["filterExpression"];
                        machine.filterExpression = tmp;
                    }

                    tempJSON["data"] = {{ "success", true }};
                }
                else
                {
                    tempJSON["error"] = {{ "success", false }};
                }

                this->resendDelayed(queueIt->second);

                this->controller.filterChangeQueue.erase(queueIt);
            }
        }
    }
    else
    {
        std::cout << "Invalid response received" << std::endl;
    }

}

void tin::controllers::main::MainVisitor::visit(events::JSONRequestReceived &evt)
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
                    { "lastSync", it.second.lastSynchronization },
                    { "filterDevice", it.second.filterDevice },
                    { "filterExpression", it.second.filterExpression }
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
                        { "lastSync", machine.lastSynchronization },
                        { "filterDevice", machine.filterDevice },
                        { "filterExpression", machine.filterExpression }
                    };
                }
                else if (action == "" && type == "PATCH")
                {
                    if(!temp["data"].is_object() ||
                        (
                            !temp["data"]["ip"].is_string() &&
                            !temp["data"]["name"].is_string() &&
                            !temp["data"]["port"].is_number() &&
                            !temp["data"]["filterDevice"].is_string() &&
                            !temp["data"]["filterExpression"].is_string()
                        )
                    )
                    {
                        std::cout << "[Supervisor] No data in PATCH" << std::endl;

                        temp["error"] = {{ "invalid", { {"data", true} } }};

                        this->resendEvent(evt);
                        return;
                    }

                    bool filterChange = false;
                    if (temp["data"]["ip"].is_string())
                    {
                        std::string tmp = temp["data"]["ip"];
                        if (tmp.length() > 0)
                        {
                            machine.ip = tmp;
                        }
                    }
                    if (temp["data"]["name"].is_string())
                    {
                        std::string tmp = temp["data"]["name"];
                        if (tmp.length() > 0)
                        {
                            machine.name = tmp;
                        }
                    }
                    if (temp["data"]["port"].is_number())
                    {
                        unsigned int tmp = temp["data"]["port"];
                        if (tmp > 0)
                        {
                            machine.port = tmp;
                        }
                    }
                    if (temp["data"]["filterDevice"].is_string() || temp["data"]["filterExpression"].is_string())
                    {
                        std::string tmpDevice = machine.filterDevice;
                        std::string tmpExpression = machine.filterExpression;

                        if (temp["data"]["filterDevice"].is_string())
                        {
                            tmpDevice = temp["data"]["filterDevice"].get<std::string>();
                        }
                        if (temp["data"]["filterExpression"].is_string())
                        {
                            tmpExpression = temp["data"]["filterExpression"].get<std::string>();
                        }

                        if (tmpDevice != machine.filterDevice || tmpExpression != machine.filterExpression)
                        {
                            filterChange = true;

                            this->controller.filterChangeQueue.insert({
                                std::pair<std::string, unsigned int>(machine.ip, machine.port),
                                std::tuple<bool, unsigned int, tin::utils::json::ptr>(evt.fromTerminal, evt.connectionID, evt.jsonPtr)
                            });

                            this->controller.bsdManagerQueue.push(
                                std::make_shared<bsdsocket::events::MessageRequest>(
                                    machine.ip,
                                    machine.port,
                                    tin::utils::json::makeSharedInstance(
                                        std::string("{ \"cmd\": \"change_filter\", \"device\": \"")
                                        .append(tmpDevice)
                                        .append("\", \"expression\": \"")
                                        .append(tmpExpression)
                                        .append(std::string("\" }"))
                                    ),
                                    true
                                )
                            );

                            return;
                        }
                    }

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "" && type == "DELETE")
                {
                    this->controller.machines.removeMachine(machineID);

                    temp["data"] = {{ "success", true }};
                }
                else if (action == "ping" && type == "POST")
                {
                    this->controller.pingsQueue.insert({
                        std::pair<std::string, unsigned int>(machine.ip, machine.port),
                        std::tuple<bool, unsigned int, tin::utils::json::ptr>(evt.fromTerminal, evt.connectionID, evt.jsonPtr)
                    });

                    this->controller.bsdManagerQueue.push(
                        std::make_shared<bsdsocket::events::MessageRequest>(
                            machine.ip,
                            machine.port,
                            tin::utils::json::makeSharedInstance("{ \"cmd\": \"ping\" }"),
                            true
                        )
                    );

                    return;
                }
                else if (action == "sync" && type == "POST")
                {
                    // Do synchronization

                    this->controller.syncQueue.insert({
                        std::pair<std::string, unsigned int>(machine.ip, machine.port),
                        std::tuple<bool, unsigned int, tin::utils::json::ptr>(evt.fromTerminal, evt.connectionID, evt.jsonPtr)
                    });

                    this->controller.bsdManagerQueue.push(
                        std::make_shared<bsdsocket::events::MessageRequest>(
                            machine.ip,
                            machine.port,
                            tin::utils::json::makeSharedInstance("{ \"cmd\": \"sync\" }"),
                            true
                        )
                    );

                    // auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                    //     std::chrono::system_clock::now().time_since_epoch()
                    // );

                    // machine.lastSynchronization = ms.count();

                    // temp["data"] = {{ "success", true }};
                    return;
                }
                else if (action == "toggle-sniffer" && type == "POST")
                {
                    // Toggle sniffer
                    std::string cmdJson;
                    if (machine.status == "sniffing")
                    {
                        cmdJson = "{ \"cmd\": \"stopsniffing\" }";
                    }
                    else if (machine.status == "stand-by")
                    {
                        cmdJson = "{ \"cmd\": \"startsniffing\" }";
                    }
                    else
                    {
                        temp["error"] = {{ "invalid", { {"status", machine.status} } }};
                        this->resendEvent(evt);
                        return;
                    }

                    this->controller.snifferToggleQueue.insert({
                        std::pair<std::string, unsigned int>(machine.ip, machine.port),
                        std::tuple<bool, unsigned int, tin::utils::json::ptr>(evt.fromTerminal, evt.connectionID, evt.jsonPtr)
                    });

                    this->controller.bsdManagerQueue.push(
                        std::make_shared<bsdsocket::events::MessageRequest>(
                            machine.ip,
                            machine.port,
                            tin::utils::json::makeSharedInstance(cmdJson),
                            true
                        )
                    );

                    return;
                }
                else
                {
                    temp["error"] = {{ "invalid", { { "action", action } } }};
                }
            }
            catch (std::out_of_range& e)
            {
                temp["error"] = {{ "invalid", { {"machineID", true} } }};
            }
        }
    }
    catch (std::exception& e)
    {
        temp["error"] = {{ "unknown", true }};
    }

    this->resendEvent(evt);
}

void tin::controllers::main::MainVisitor::resendEvent(events::JSONRequestReceived &evt)
{
    if (!evt.fromTerminal)
    {
        this->controller.networkManagerQueue.push(
            std::make_shared<websocket::events::MessageSendRequest>(evt.connectionID, evt.jsonPtr)
        );
    }
    else
    {
        this->controller.terminalQueue.push(
            std::make_shared<terminal::events::SendMessage>(evt.jsonPtr)
        );
    }
}

void tin::controllers::main::MainVisitor::resendDelayed(std::tuple<bool, unsigned int, tin::utils::json::ptr> obj)
{
    if (!std::get<0>(obj))
    {
        this->controller.networkManagerQueue.push(
            std::make_shared<websocket::events::MessageSendRequest>(std::get<1>(obj), std::get<2>(obj))
        );
    }
    else
    {
        this->controller.terminalQueue.push(
            std::make_shared<terminal::events::SendMessage>(std::get<2>(obj))
        );
    }
}

void tin::controllers::main::MainVisitor::visit(events::NetworkRequest& evt)
{
    this->controller.bsdManagerQueue.push(std::make_shared<tin::network::bsdsocket::events::MessageRequest>(evt.ip, evt.port, evt.jsonPtr, true));
}

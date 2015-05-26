#include "ManagerVisitor.hpp"

#include "Manager.hpp"

#include "events/TerminateNetworkManager.hpp"
#include "events/NewServerConnection.hpp"
#include "events/ServerConnectionClosed.hpp"
#include "events/MessageReceived.hpp"
#include "events/MessageSendRequest.hpp"
#include "events/MessageSendMultiRequest.hpp"
#include "events/MessageBroadcastRequest.hpp"
#include "../../controllers/main/events/WebClientRequestReceived.hpp"

#include "../../utils/JSON.hpp"

using tin::network::websocket::ManagerVisitor;
namespace events = tin::network::websocket::events;

#include <iostream>
#include <tuple>
#include <map>
#include <stdexcept>
#include <chrono>

void ManagerVisitor::visit(events::TerminateNetworkManager& evt)
{
    this->manager.terminate();
}

void ManagerVisitor::visit(events::NewServerConnection& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::ServerConnectionClosed& evt)
{
    // this->manager.controllerQueue.push(
    //     tin::controllers::main::EventPtr(
    //     )
    // );
}

void ManagerVisitor::visit(events::MessageReceived& evt)
{
    //static unsigned int lastID = 0;
    //static std::map<unsigned int, std::tuple<std::string, std::string, unsigned int, std::string, unsigned int>> machines;

    std::cout << "[WebSockets] Received message: " << std::endl;
    std::cout << evt.message << std::endl;

    auto jsonObj = nlohmann::json::parse(evt.message);

    try
    {
        if (!jsonObj["route"].is_string() || !jsonObj["type"].is_string())
        {
            jsonObj["error"] = {{ "invalid", { {"route", "type"} } }};

            this->manager.server.sendMessage(evt.serverConnectionID, jsonObj.dump());
            return;
        }

        std::string route = jsonObj["route"];
        std::string type = jsonObj["type"];

        nlohmann::json temp;
        temp["route"] = route;
        temp["type"] = type;

        if (route == "machine")
        {
            if (type == "GET")
            {
                this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));

            }
            else if (type == "POST")
            {
                std::string name = jsonObj["data"]["name"];
                std::string ip = jsonObj["data"]["ip"];
                unsigned int port = jsonObj["data"]["port"];

                temp["data"]["name"] = name;
                temp["data"]["ip"] = ip;
                temp["data"]["port"] = port;

                this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));
            }
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

            unsigned int machineID;
            try
            {
               machineID = static_cast<unsigned int>(std::stoul(routeRest));

               // auto& machine = machines.at(machineID);

                if (action == "" && type == "GET")
                {
                    this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));

                    /*jsonObj["data"] = {
                        { "id", machineID },
                        { "name", std::get<0>(machine) },
                        { "ip", std::get<1>(machine) },
                        { "port", std::get<2>(machine) },
                        { "status", std::get<3>(machine) },
                        { "lastSync", std::get<4>(machine) }*/
                    
                }
                else if (action == "" && type == "PATCH")
                {
                    this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));

                    std::string name = jsonObj["data"]["name"];
                    std::string ip = jsonObj["data"]["ip"];
                    unsigned int port = jsonObj["data"]["port"];

                    temp["data"]["name"] = name;
                    temp["data"]["ip"] = ip;
                    temp["data"]["port"] = port;

                    /*
                    std::get<0>(machine) = name;
                    std::get<1>(machine) = ip;
                    std::get<2>(machine) = port;

                    jsonObj["data"] = {{ "success", true }};*/
                }
                else if (action == "" && type == "DELETE")
                {
                    this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));
                    
                    //jsonObj["data"] = {{ "success", true }};
                }
                else if (action == "sync" && type == "POST")
                {
                    /*auto ms = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    );

                    std::get<4>(machine) = ms.count();*/
                    this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));

                    //jsonObj["data"] = {{ "success", true }};
                }
                else if (action == "toggle-sniffer" && type == "POST")
                {
                    this->manager.controllerQueue.push(tin::controllers::main::EventPtr(
                    new tin::controllers::main::events::WebClientRequestReceived
                    (tin::utils::json::makeSharedInstance(temp), evt.serverConnectionID)));

                    /*
                    if (std::get<3>(machine) == "sniffing")
                    {
                        std::get<3>(machine) = "stand-by";
                        jsonObj["data"] = {{ "success", true }};
                    }
                    else if (std::get<3>(machine) == "stand-by")
                    {
                        std::get<3>(machine) = "sniffing";
                        jsonObj["data"] = {{ "success", true }};
                    }
                    else
                    {
                        jsonObj["error"] = {{ "invalid", { {"status", std::get<3>(machine)} } }};
                    } */
                }
                else
                {
                    jsonObj["error"] = {{ "invalid", { {"action", action} } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                jsonObj["error"] = {{ "invalid", { {"routeID", true} } }};
            }
            catch (std::out_of_range& e)
            {
                jsonObj["error"] = {{ "notFound", { {"machine", machineID} } }};
            }
        }
        /*else if (route == "stats-per-day")
        {
            if (type == "GET")
            {
                unsigned int lastDays = 7;

                if (jsonObj["data"].is_object() && jsonObj["data"]["lastDays"].is_number())
                {
                    lastDays = jsonObj["data"]["lastDays"];
                }

                auto day = nlohmann::json();
                day = {
                    { "day", 1432404865 },
                    { "machines", {
                        {
                            { "id", 1 },
                            { "name", "Machine test" },
                            { "traffic", {
                                { "out", 12 },
                                { "in", 3 }
                            } }
                        },
                        {
                            { "id", 2 },
                            { "name", "Machine ubuntu" },
                            { "traffic", {
                                { "out", 24 },
                                { "in", 17 }
                            } }
                        }
                    }}
                };

                jsonObj["data"] = {
                    { "stats", {} }
                };

                for(int i = 0; i < lastDays; ++i)
                {
                    jsonObj["data"]["stats"][i] = day;
                    jsonObj["data"]["stats"][i]["day"] = ((int) jsonObj["data"]["stats"][i]["day"]) + (i * 24 * 60 * 60);
                    jsonObj["data"]["stats"][i]["machines"][0]["traffic"]["out"] = ((int) jsonObj["data"]["stats"][i]["machines"][0]["traffic"]["out"]) + (5 * (i % 2 == 1 ? -1 : 1));
                    jsonObj["data"]["stats"][i]["machines"][0]["traffic"]["in"] = ((int) jsonObj["data"]["stats"][i]["machines"][0]["traffic"]["in"]) + (5 * (i % 2 == 1 ? -1 : 1));
                    jsonObj["data"]["stats"][i]["machines"][1]["traffic"]["out"] = ((int) jsonObj["data"]["stats"][i]["machines"][1]["traffic"]["out"]) + (2 * (i % 2 == 0 ? -1 : 1));
                    jsonObj["data"]["stats"][i]["machines"][1]["traffic"]["in"] = ((int) jsonObj["data"]["stats"][i]["machines"][1]["traffic"]["in"]) + (2 * (i % 2 == 0 ? -1 : 1));
                }
            }
        }
        else if (route == "stats-per-machine")
        {
            if (type == "GET")
            {
                unsigned int lastDays = 7;

                if (jsonObj["data"].is_object() && jsonObj["data"]["lastDays"].is_number())
                {
                    lastDays = jsonObj["data"]["lastDays"];
                }

                auto machine = nlohmann::json();
                machine = {
                    { "id", 0 },
                    { "name", "Linux Machine " },
                    { "traffic", {
                        { "out", 12 },
                        { "in", 7 }
                    } }
                };

                jsonObj["data"] = {
                    { "machines", {} }
                };

                for(int i = 0; i < lastDays; ++i)
                {
                    jsonObj["data"]["machines"][i] = machine;
                    jsonObj["data"]["machines"][i]["id"] = ((int) jsonObj["data"]["machines"][i]["id"]) + (i);
                    jsonObj["data"]["machines"][i]["traffic"]["out"] = ((int) jsonObj["data"]["machines"][i]["traffic"]["out"]) + (2 * (i % 2 == 0 ? -1 : 1));
                    jsonObj["data"]["machines"][i]["traffic"]["in"] = ((int) jsonObj["data"]["machines"][i]["traffic"]["in"]) + (2 * (i % 2 == 0 ? -1 : 1));
                }
            }
        }
        else if (route.substr(0, 22) == "stats-machine-per-day/")
        {
            auto routeRest = route.substr(22);
            auto action = std::string("");
            if (routeRest.find("/") != std::string::npos)
            {
                action = routeRest.substr(routeRest.find("/") + 1);
                routeRest = routeRest.substr(0, routeRest.find("/"));
            }

            unsigned int machineID;
            try
            {
                machineID = static_cast<unsigned int>(std::stoul(routeRest));

                auto& machine = machines.at(machineID);

                if (action == "" && type == "GET")
                {
                    unsigned int lastDays = 7;

                    if (jsonObj["data"].is_object() && jsonObj["data"]["lastDays"].is_number())
                    {
                        lastDays = jsonObj["data"]["lastDays"];
                    }

                    jsonObj["data"] = {
                        { "stats", {} }
                    };

                    for(int i = 0; i < lastDays; ++i)
                    {
                        jsonObj["data"]["stats"][i] = {};
                        jsonObj["data"]["stats"][i]["day"] = (1432404865) + (i * 24 * 60 * 60);
                        jsonObj["data"]["stats"][i]["traffic"] = {};
                        jsonObj["data"]["stats"][i]["traffic"]["out"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["traffic"]["in"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["packets"] = {};
                        jsonObj["data"]["stats"][i]["packets"]["out"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["packets"]["in"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                    }
                }
                else
                {
                    jsonObj["error"] = {{ "invalid", { {"action", action} } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                jsonObj["error"] = {{ "invalid", { {"routeID", true} } }};
            }
            catch (std::out_of_range& e)
            {
                jsonObj["error"] = {{ "notFound", { {"machine", machineID} } }};
            }
        }
        else if (route.substr(0, 29) == "stats-machine-per-connection/")
        {
            auto routeRest = route.substr(29);
            auto action = std::string("");
            if (routeRest.find("/") != std::string::npos)
            {
                action = routeRest.substr(routeRest.find("/") + 1);
                routeRest = routeRest.substr(0, routeRest.find("/"));
            }

            unsigned int machineID;
            try
            {
                machineID = static_cast<unsigned int>(std::stoul(routeRest));

                auto& machine = machines.at(machineID);

                if (action == "" && type == "GET")
                {
                    unsigned int lastDays = 7;

                    if (jsonObj["data"].is_object() && jsonObj["data"]["lastDays"].is_number())
                    {
                        lastDays = jsonObj["data"]["lastDays"];
                    }

                    jsonObj["data"] = {
                        { "connections", {} }
                    };

                    for(int i = 0; i < lastDays; ++i)
                    {
                        jsonObj["data"]["connections"][i] = {};
                        jsonObj["data"]["connections"][i]["ip"] = (1270000) + (i * 5);
                        jsonObj["data"]["connections"][i]["port"] = 3333;
                        jsonObj["data"]["connections"][i]["lastTime"] = (1432404865) + (i * 8 * 60 * 60);
                        jsonObj["data"]["connections"][i]["traffic"] = {};
                        jsonObj["data"]["connections"][i]["traffic"]["out"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["connections"][i]["traffic"]["in"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["connections"][i]["packets"] = {};
                        jsonObj["data"]["connections"][i]["packets"]["out"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["connections"][i]["packets"]["in"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                    }
                }
                else
                {
                    jsonObj["error"] = {{ "invalid", { {"action", action} } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                jsonObj["error"] = {{ "invalid", { {"routeID", true} } }};
            }
            catch (std::out_of_range& e)
            {
                jsonObj["error"] = {{ "notFound", { {"machine", machineID} } }};
            }
        }
        else if (route.substr(0, 23) == "stats-machine-per-hour/")
        {
            auto routeRest = route.substr(23);
            auto action = std::string("");
            if (routeRest.find("/") != std::string::npos)
            {
                action = routeRest.substr(routeRest.find("/") + 1);
                routeRest = routeRest.substr(0, routeRest.find("/"));
            }

            unsigned int machineID;
            try
            {
                machineID = static_cast<unsigned int>(std::stoul(routeRest));

                auto& machine = machines.at(machineID);

                if (action == "" && type == "GET")
                {
                    unsigned int lastDays = 7;

                    if (jsonObj["data"].is_object() && jsonObj["data"]["lastDays"].is_number())
                    {
                        lastDays = jsonObj["data"]["lastDays"];
                    }

                    jsonObj["data"] = {
                        { "stats", {} }
                    };

                    for(int i = 0; i < 24; ++i)
                    {
                        jsonObj["data"]["stats"][i] = {};
                        jsonObj["data"]["stats"][i]["hour"] = i;
                        jsonObj["data"]["stats"][i]["traffic"] = {};
                        jsonObj["data"]["stats"][i]["traffic"]["out"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["traffic"]["in"] = (12) + (2 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["packets"] = {};
                        jsonObj["data"]["stats"][i]["packets"]["out"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                        jsonObj["data"]["stats"][i]["packets"]["in"] = (102) + (100 * (i % 2 == 0 ? -1 : 1));
                    }
                }
                else
                {
                    jsonObj["error"] = {{ "invalid", { {"action", action} } }};
                }
            }
            catch (std::invalid_argument& e)
            {
                jsonObj["error"] = {{ "invalid", { {"routeID", true} } }};
            }
            catch (std::out_of_range& e)
            {
                jsonObj["error"] = {{ "notFound", { {"machine", machineID} } }};
            }
        }
        else
        {
            jsonObj["error"] = {{ "invalid", { {"route", route} } }};
        }*/
    }
    
    catch(std::exception& e)
    {
        jsonObj["error"] = {{ "unknown", true }};
    }

    this->manager.server.sendMessage(evt.serverConnectionID, jsonObj.dump());
}

void ManagerVisitor::visit(events::MessageSendRequest& evt)
{
    this->manager.server.sendMessage(
        evt.serverConnectionID,
        "empty"
    );
}

void ManagerVisitor::visit(events::MessageSendMultiRequest& evt)
{
    this->manager.server.sendMessage(
        evt.serverConnectionIDs,
        "empty"
    );
}

void ManagerVisitor::visit(events::MessageBroadcastRequest& evt)
{
    this->manager.server.broadcastMessage(
        "empty"
    );
}

ManagerVisitor::ManagerVisitor(tin::network::websocket::Manager& manager):
manager(manager)
{}
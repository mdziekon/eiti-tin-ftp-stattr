#include "StatsVisitor.hpp"

#include "Stats.hpp"

#include <arpa/inet.h>
#include <string>
#include <sys/types.h>

#include "events/ReceivePackets.hpp"
#include "events/RequestAnalytics.hpp"

#include "../controllers/main/events/NetworkRequest.hpp"
#include "../controllers/main/events/WebClientSendRequest.hpp"

using namespace tin::supervisor::models;

void StatsVisitor::visit(events::ReceivePackets& event)
{
    this->stats.appendPackets((*event.packetsArray));
}

void StatsVisitor::visit(events::RequestAnalytics& event)
{
    tin::utils::json::ptr computedStats;

    std::string route = (*event.requestData)["route"].get<std::string>();
    std::string type = (*event.requestData)["type"].get<std::string>();

    if(route == "stats-per-day") {
        computedStats = this->stats.computeStatsPerDay(event.requestData);
    }
    else if(route == "stats-per-machine") {
        computedStats = this->stats.computeIndividualUsage(event.requestData);
    }
    else if (route.substr(0, 22) == "stats-machine-per-day/")
    {
        computedStats = event.requestData;

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

            auto& machine = this->stats.machines.getMachine(machineID);

            if (action == "" && type == "GET")
            {
                computedStats = this->stats.computeMachinesPerDay(event.requestData, machineID);
            }
            else
            {
                (*computedStats)["error"] = {{ "invalid", { {"action", action} } }};
            }
        }
        catch (std::invalid_argument& e)
        {
            (*computedStats)["error"] = {{ "invalid", { {"routeID", true} } }};
        }
        catch (std::out_of_range& e)
        {
            (*computedStats)["error"] = {{ "notFound", { {"machine", machineID} } }};
        }
    }
    else if (route.substr(0, 29) == "stats-machine-per-connection/")
    {
        computedStats = event.requestData;

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

            auto& machine = this->stats.machines.getMachine(machineID);

            if (action == "" && type == "GET")
            {
                computedStats = this->stats.computeMachinesPerConnection(event.requestData, machineID);
            }
            else
            {
                (*computedStats)["error"] = {{ "invalid", { {"action", action} } }};
            }
        }
        catch (std::invalid_argument& e)
        {
            (*computedStats)["error"] = {{ "invalid", { {"routeID", true} } }};
        }
        catch (std::out_of_range& e)
        {
            (*computedStats)["error"] = {{ "notFound", { {"machine", machineID} } }};
        }
    }
    else if (route.substr(0, 23) == "stats-machine-per-hour/")
    {
        computedStats = event.requestData;

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

            auto& machine = this->stats.machines.getMachine(machineID);

            if (action == "" && type == "GET")
            {
                computedStats = this->stats.computeMachinesPerHour(event.requestData, machineID);
            }
            else
            {
                (*computedStats)["error"] = {{ "invalid", { {"action", action} } }};
            }
        }
        catch (std::invalid_argument& e)
        {
            (*computedStats)["error"] = {{ "invalid", { {"routeID", true} } }};
        }
        catch (std::out_of_range& e)
        {
            (*computedStats)["error"] = {{ "notFound", { {"machine", machineID} } }};
        }
    }
    else
    {
        computedStats = event.requestData;
        (*computedStats)["error"] = {{ "invalid", { {"action", true } } }};
        std::cout << "[Supervisor] Invalid or unknown analytics request received" << std::endl;
    }
    
    this->stats.controllerQueue.push(
        std::make_shared<tin::controllers::main::events::WebClientSendRequest>(
            computedStats,
            event.connectionID
        )
    );
}

StatsVisitor::StatsVisitor(tin::supervisor::models::Stats& stats):
stats(stats)
{}

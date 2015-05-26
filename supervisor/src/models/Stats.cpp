#include "Stats.hpp"

#include <ctime>
#include <iostream>
#include <arpa/inet.h>
#include <unordered_map>

#include "StatsVisitor.hpp"
#include "../controllers/main/events/NetworkRequest.hpp"

using namespace tin::supervisor::models;

const tin::utils::json::ptr Stats::computeStatsPerDay(const tin::utils::json::ptr& requestorData) const
{
    tin::utils::json::ptr reply = std::make_shared<nlohmann::json>(nlohmann::json::object());
    
    std::unordered_map<u_int32_t, u_int32_t> machineStats;

    (*reply)["route"] = (*requestorData)["route"];
    (*reply)["type"] = "GET";
    (*reply)["uid"] = (*requestorData)["uid"];
    (*reply)["data"] = { {"stats", nlohmann::json::array() }};
    

    if(this->packetsByTimestamp.size() > 0)
    {
        struct tm packetDay;
        int64_t beginTime = this->packetsByTimestamp.begin()->first;
        struct tm currentDay = *localtime(&beginTime);
        u_int32_t arrayIndex = 0;
        for(auto connectionDataPair: this->packetsByTimestamp) 
        {
            auto& connectionData = connectionDataPair.second;
            u_int32_t sourceIP = (*connectionData)["sourceIP"];
            u_int32_t destinationIP = (*connectionData)["destinationIP"];
            
            if(machineStats.find(sourceIP) != machineStats.end()) {
                machineStats[sourceIP]++;
            }
            else {
                machineStats[sourceIP] = 1;
            }
            if(machineStats.find(destinationIP) != machineStats.end()) {
                machineStats[destinationIP]++;
            }
            else {
                machineStats[destinationIP] = 1;
            }

            int64_t temporaryTimeStorage = connectionDataPair.first;
            packetDay = *localtime(&temporaryTimeStorage);
            
            if(packetDay.tm_mday > currentDay.tm_mday ||
                packetDay.tm_year > currentDay.tm_year)
            {
                u_int32_t innerMachineID = 0;
                
                for(auto stat: machineStats) 
                {
                    (*reply)["data"]["stats"][arrayIndex]["day"] = mktime(&currentDay);
                    (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["id"] = innerMachineID;
                    (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["name"] = stat.first;
                    (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["traffic"] = stat.second;
                    innerMachineID++;
                }
                currentDay = packetDay;
                arrayIndex++;
            }
        }
        u_int32_t innerMachineID = 0;
        
        for(auto stat: machineStats) 
        {
            (*reply)["data"]["stats"][arrayIndex]["day"] = mktime(&currentDay);
            (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["id"] = innerMachineID;
            (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["name"] = stat.first;
            (*reply)["data"]["stats"][arrayIndex]["machines"][innerMachineID]["traffic"] = stat.second;
            innerMachineID++;
        }
    }
    else {
        (*reply)["data"] = { {"stats", nlohmann::json::array() }};
    }

    // std::cout << "---- COMPUTE1 ---- " << std::endl;
    // std::cout << (*reply).dump() << std::endl;
    // std::cout << "---- COMPUTE ---- " << std::endl;

    return reply;
}

const tin::utils::json::ptr Stats::computeIndividualUsage(const tin::utils::json::ptr& requestorData) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    std::unordered_map<u_int32_t, u_int32_t> machineStats;

    (*reply)["route"] = (*requestorData)["route"];
    (*reply)["type"] = "GET";
    (*reply)["uid"] = (*requestorData)["uid"];
    (*reply)["data"] = { {"machines", nlohmann::json::array() }};
    
    for(auto connectionDataPair: this->packetsByTimestamp) 
    {
        auto& connectionData = connectionDataPair.second;
        u_int32_t sourceIP = (*connectionData)["sourceIP"];
        u_int32_t destinationIP = (*connectionData)["destinationIP"];
        
        if(machineStats.find(sourceIP) != machineStats.end()) {
            machineStats[sourceIP] += (*connectionData)["payoadSize"].get<u_int32_t>();
        }
        else {
            machineStats[sourceIP] = (*connectionData)["payoadSize"].get<u_int32_t>();
        }
        if(machineStats.find(destinationIP) != machineStats.end()) {
            machineStats[destinationIP] += (*connectionData)["payoadSize"].get<u_int32_t>();
        }
        else {
            machineStats[destinationIP] = (*connectionData)["payoadSize"].get<u_int32_t>();
        }
    }
    
    u_int32_t innerMachineID = 0;
    
    for(auto stat: machineStats) 
    {
        (*reply)["data"]["machines"][innerMachineID]["id"] = innerMachineID;
        (*reply)["data"]["machines"][innerMachineID]["name"] = stat.first;
        (*reply)["data"]["machines"][innerMachineID]["traffic"] = stat.second;
        innerMachineID++;
    }
    
    // std::cout << "---- COMPUTE2 ---- " << std::endl;
    // std::cout << (*reply).dump() << std::endl;
    // std::cout << "---- COMPUTE ---- " << std::endl;

    return reply;
}

std::thread Stats::createRequestorThread(
    const u_int32_t& intervalMilliseconds, 
    tin::controllers::main::ControllerQueue& controlerQueue
)
{
    return std::thread([&]() {
        while(1) {
            std::cout << "Helper thread active\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliseconds));
            tin::utils::json::ptr requestJson(new nlohmann::json);
            (*requestJson)["cmd"] = "fetch_packets";
            controllerQueue.push(
                std::make_shared<tin::controllers::main::events::NetworkRequest>(
                    "localhost",
                    3333,
                    requestJson
                )
            );
        }
    });
}

Stats::Stats(
    tin::supervisor::models::StatsQueue& statsQueue,
    tin::controllers::main::ControllerQueue& controllerQueue
): 
QueueThread< tin::supervisor::models::Event, tin::supervisor::models::StatsVisitor >(statsQueue, StatsVisitor(*this)),
controllerQueue(controllerQueue)
{}

#include "Stats.hpp"

#include <ctime>
#include <iostream>
#include <arpa/inet.h>
#include <unordered_map>
#include <unordered_set>

#include "StatsVisitor.hpp"
#include "../controllers/main/events/NetworkRequest.hpp"

using namespace tin::supervisor::models;

Stats::Stats(
    tin::supervisor::models::StatsQueue& statsQueue,
    tin::controllers::main::ControllerQueue& controllerQueue,
    tin::models::MachinesStorage& machines
):
QueueThread< tin::supervisor::models::Event, tin::supervisor::models::StatsVisitor >(statsQueue, StatsVisitor(*this)),
controllerQueue(controllerQueue),
machines(machines)
{
    this->packetsFileOpen("packets.json");
    this->packetsFileLoadData();
}

void Stats::packetsFileOpen(const std::string &filename)
{
    this->packetsFile.open(filename);
    this->lastPacketsFile = filename;

    if (!this->packetsFile)
    {
        this->packetsFile.open(filename, std::ios_base::out | std::ios_base::trunc);
        this->packetsFile.close();
        this->packetsFile.open(filename);
    }

    if (!this->packetsFile.is_open())
    {
        std::cout << "[Stats] Could not open packetsFile (" << filename << ")!!!" << std::endl;
        this->lastPacketsFile = "";
        return;
    }
}

void Stats::packetsFileLoadData()
{
    if (!this->packetsFile.is_open())
    {
        return;
    }

    try
    {
        std::string fileContent;

        this->packetsFile.seekg(0, std::ios::end);
        fileContent.reserve(this->packetsFile.tellg());
        this->packetsFile.seekg(0, std::ios::beg);

        fileContent.assign(
            (std::istreambuf_iterator<char>(this->packetsFile)),
            std::istreambuf_iterator<char>()
        );

        nlohmann::json loadedJSON = nlohmann::json::parse(fileContent);

        if (!loadedJSON.is_array())
        {
            std::cout << "[Stats] 'Not an array' in packetsFile (" << this->lastPacketsFile << ")!!!" << std::endl;
            return;
        }

        for(auto& packetJSON: loadedJSON)
        {
            this->packets.push_back(tin::utils::Packet(packetJSON));
        }
    }
    catch (std::exception& e)
    {
        std::cout << "[Stats] Invalid data in packetsFile (" << this->lastPacketsFile << ")!!!" << std::endl;
    }
}

void Stats::packetsFileAppendData(nlohmann::json &packetsArray)
{
    if (!this->packetsFile.is_open())
    {
        return;
    }

    this->packetsFile.close();
    this->packetsFile.open(this->lastPacketsFile, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    if (!this->packetsFile.is_open())
    {
        std::cout << "[Stats] Could not reopen packetsFile (" << this->lastPacketsFile << ")!!!" << std::endl;
        return;
    }

    nlohmann::json dataJSON = nlohmann::json::array();
    for(auto& packet: this->packets)
    {
        dataJSON[dataJSON.size()] = packet.serialize();
    }

    this->packetsFile << dataJSON.dump();
}

void Stats::appendPackets(nlohmann::json &packetsArray)
{
    for(auto& packetJSON: packetsArray)
    {
        this->packets.push_back(tin::utils::Packet(packetJSON));
    }

    this->packetsFileAppendData(packetsArray);
}

const tin::utils::json::ptr Stats::computeStatsPerDay(const tin::utils::json::ptr& requestorData) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    try
    {
        std::unordered_map<u_int32_t, u_int32_t> machineStats;

        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["data"] = { {"stats", nlohmann::json::array() }};

        std::unordered_map<int, nlohmann::json> dStats;
        std::unordered_map<int, std::unordered_map<int, nlohmann::json>> mStats;

        for(auto& it: this->packets)
        {
            int dayTimestamp = it.timestamp;
            dayTimestamp -= dayTimestamp % (24 * 60 * 60);

            if (mStats.count(dayTimestamp) == 0)
            {
                mStats.insert({ dayTimestamp, std::unordered_map<int, nlohmann::json>() });
            }

            auto& thisMStats = mStats.at(dayTimestamp);

            if (this->machines.hasMachine(it.getSourceIP()))
            {
                auto& machine = this->machines.getMachine(it.getSourceIP());
                if (thisMStats.count(machine.id) == 0)
                {
                    thisMStats.insert({ machine.id, nlohmann::json::object() });
                    thisMStats.at(machine.id) = {
                        { "id", machine.id() },
                        { "name", machine.name() },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = thisMStats.at(machine.id);

                jsObj["traffic"]["out"] = (jsObj["traffic"]["out"].get<int>() + it.payloadSize);
            }
            else if (this->machines.hasMachine(it.getDestinationIP()))
            {
                auto& machine = this->machines.getMachine(it.getDestinationIP());
                if (thisMStats.count(machine.id) == 0)
                {
                    thisMStats.insert({ machine.id, nlohmann::json::object() });
                    thisMStats.at(machine.id) = {
                        { "id", machine.id() },
                        { "name", machine.name() },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = thisMStats.at(machine.id);

                jsObj["traffic"]["in"] = (jsObj["traffic"]["in"].get<int>() + it.payloadSize);
            }
        }

        for(auto& itt: mStats)
        {
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size()] = {
                { "day", itt.first },
                { "machines", nlohmann::json::array() }
            };

            for(auto& ittt: itt.second)
            {
                (*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["machines"][(*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["machines"].size()] = ittt.second;
            }

            // (*reply)["data"]["machines"][(*reply)["data"]["machines"].size()] = itt.second;
        }
    }
    catch (std::exception& e)
    {
        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["error"] = { {"unknown", true }};
    }

    return reply;
}

const tin::utils::json::ptr Stats::computeIndividualUsage(const tin::utils::json::ptr& requestorData) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    try
    {
        std::unordered_map<u_int32_t, u_int32_t> machineStats;

        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["data"] = { {"machines", nlohmann::json::array() }};

        std::unordered_map<int, nlohmann::json> mStats;

        for(auto& it: this->packets)
        {
            if (this->machines.hasMachine(it.getSourceIP()))
            {
                auto& machine = this->machines.getMachine(it.getSourceIP());
                if (mStats.count(machine.id) == 0)
                {
                    mStats.insert({ machine.id, nlohmann::json::object() });
                    mStats.at(machine.id) = {
                        { "id", machine.id() },
                        { "name", machine.name() },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = mStats.at(machine.id);

                jsObj["traffic"]["out"] = (jsObj["traffic"]["out"].get<int>() + it.payloadSize);
            }
            else if (this->machines.hasMachine(it.getDestinationIP()))
            {
                auto& machine = this->machines.getMachine(it.getDestinationIP());
                if (mStats.count(machine.id) == 0)
                {
                    mStats.insert({ machine.id, nlohmann::json::object() });
                    mStats.at(machine.id) = {
                        { "id", machine.id() },
                        { "name", machine.name() },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = mStats.at(machine.id);

                jsObj["traffic"]["in"] = (jsObj["traffic"]["in"].get<int>() + it.payloadSize);
            }
        }

        for(auto& itt: mStats)
        {
            (*reply)["data"]["machines"][(*reply)["data"]["machines"].size()] = itt.second;
        }
    }
    catch (std::exception& e)
    {
        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["error"] = { {"unknown", true }};
    }

    return reply;
}

const tin::utils::json::ptr Stats::computeMachinesPerDay(const tin::utils::json::ptr& requestorData, int machineID) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    try
    {
        std::unordered_map<u_int32_t, u_int32_t> machineStats;

        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["data"] = { {"stats", nlohmann::json::array() }};

        tin::utils::Machine& machine = this->machines.getMachine(machineID);
        std::unordered_map<int, nlohmann::json> dStats;

        for(auto& it: this->packets)
        {
            int dayTimestamp = it.timestamp;
            dayTimestamp -= dayTimestamp % (24 * 60 * 60);

            if (machine.ip == it.getSourceIP())
            {
                if (dStats.count(dayTimestamp) == 0)
                {
                    dStats.insert({ dayTimestamp, nlohmann::json::object() });
                    dStats.at(dayTimestamp) = {
                        { "day", dayTimestamp },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = dStats.at(dayTimestamp);

                jsObj["traffic"]["out"] = (jsObj["traffic"]["out"].get<int>() + it.payloadSize);
                jsObj["packets"]["out"] = (jsObj["packets"]["out"].get<int>() + 1);
            }
            else if (machine.ip == it.getDestinationIP())
            {
                if (dStats.count(dayTimestamp) == 0)
                {
                    dStats.insert({ dayTimestamp, nlohmann::json::object() });
                    dStats.at(dayTimestamp) = {
                        { "day", dayTimestamp },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = dStats.at(dayTimestamp);

                jsObj["traffic"]["in"] = (jsObj["traffic"]["in"].get<int>() + it.payloadSize);
                jsObj["packets"]["in"] = (jsObj["packets"]["in"].get<int>() + 1);
            }
        }

        for(auto& itt: dStats)
        {
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size()] = itt.second;
        }
    }
    catch (std::exception& e)
    {
        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["error"] = { {"unknown", true }};
    }

    return reply;
}

const tin::utils::json::ptr Stats::computeMachinesPerConnection(const tin::utils::json::ptr& requestorData, int machineID) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    try
    {
        std::unordered_map<u_int32_t, u_int32_t> machineStats;

        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["data"] = { {"connections", nlohmann::json::array() }};

        tin::utils::Machine& machine = this->machines.getMachine(machineID);
        std::unordered_map<std::string, nlohmann::json> cStats;

        for(auto& it: this->packets)
        {
            std::string key;

            if (machine.ip == it.getSourceIP())
            {
                key = std::string(it.getDestinationIP()).append(std::to_string(it.destinationPort));
                if (cStats.count(key) == 0)
                {
                    cStats.insert({ key, nlohmann::json::object() });
                    cStats.at(key) = {
                        { "ip", it.getDestinationIP() },
                        { "port", it.destinationPort },
                        { "lastTime", 0 },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = cStats.at(key);

                if (jsObj["lastTime"].get<int>() < it.timestamp)
                {
                    jsObj["lastTime"] = it.timestamp;
                }
                jsObj["traffic"]["out"] = (jsObj["traffic"]["out"].get<int>() + it.payloadSize);
                jsObj["packets"]["out"] = (jsObj["packets"]["out"].get<int>() + 1);
            }
            else if (machine.ip == it.getDestinationIP())
            {
                key = std::string(it.getSourceIP()).append(std::to_string(it.sourcePort));
                if (cStats.count(key) == 0)
                {
                    cStats.insert({ key, nlohmann::json::object() });
                    cStats.at(key) = {
                        { "ip", it.getSourceIP() },
                        { "port", it.sourcePort },
                        { "lastTime", 0 },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = cStats.at(key);

                if (jsObj["lastTime"].get<int>() < it.timestamp)
                {
                    jsObj["lastTime"] = it.timestamp;
                }
                jsObj["traffic"]["in"] = (jsObj["traffic"]["in"].get<int>() + it.payloadSize);
                jsObj["packets"]["in"] = (jsObj["packets"]["in"].get<int>() + 1);
            }
        }

        for(auto& itt: cStats)
        {
            (*reply)["data"]["connections"][(*reply)["data"]["connections"].size()] = itt.second;
        }
    }
    catch (std::exception& e)
    {
        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["error"] = { {"unknown", true }};
    }

    return reply;
}

const tin::utils::json::ptr Stats::computeMachinesPerHour(const tin::utils::json::ptr& requestorData, int machineID) const
{
    tin::utils::json::ptr reply(new nlohmann::json);
    try
    {
        std::unordered_map<u_int32_t, u_int32_t> machineStats;

        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["data"] = { {"stats", nlohmann::json::array() }};

        tin::utils::Machine& machine = this->machines.getMachine(machineID);
        std::unordered_map<int, nlohmann::json> cStats;
        std::unordered_map<int, std::unordered_set<int>> days;

        for(auto& it: this->packets)
        {
            int dayTimestamp = it.timestamp;
            dayTimestamp -= dayTimestamp % (24 * 60 * 60);

            int hourTimestamp = it.timestamp;
            hourTimestamp = hourTimestamp % (24 * 60 * 60);
            hourTimestamp -= hourTimestamp % (60 * 60);
            hourTimestamp /= (60 * 60);
            std::string key;

            if (machine.ip == it.getSourceIP())
            {
                if (cStats.count(hourTimestamp) == 0)
                {
                    days.insert({ hourTimestamp, std::unordered_set<int>() });
                    cStats.insert({ hourTimestamp, nlohmann::json::object() });
                    cStats.at(hourTimestamp) = {
                        { "hour", hourTimestamp },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = cStats.at(hourTimestamp);
                days.at(hourTimestamp).insert(dayTimestamp);

                jsObj["traffic"]["out"] = (jsObj["traffic"]["out"].get<int>() + it.payloadSize);
                jsObj["packets"]["out"] = (jsObj["packets"]["out"].get<int>() + 1);
            }
            else if (machine.ip == it.getDestinationIP())
            {
                if (cStats.count(hourTimestamp) == 0)
                {
                    days.insert({ hourTimestamp, std::unordered_set<int>() });
                    cStats.insert({ hourTimestamp, nlohmann::json::object() });
                    cStats.at(hourTimestamp) = {
                        { "hour", hourTimestamp },
                        { "traffic", {
                            { "in", 0 },
                            { "out", 0 }
                        }},
                        { "packets", {
                            { "in", 0 },
                            { "out", 0 }
                        }}
                    };
                }
                auto& jsObj = cStats.at(hourTimestamp);
                days.at(hourTimestamp).insert(dayTimestamp);

                jsObj["traffic"]["in"] = (jsObj["traffic"]["in"].get<int>() + it.payloadSize);
                jsObj["packets"]["in"] = (jsObj["packets"]["in"].get<int>() + 1);
            }
        }

        for(auto& itt: cStats)
        {
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size()] = itt.second;
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["traffic"]["in"] = ((*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["traffic"]["in"].get<int>() / days.at(itt.first).size());
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["traffic"]["out"] = ((*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["traffic"]["out"].get<int>() / days.at(itt.first).size());
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["packets"]["in"] = ((*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["packets"]["in"].get<int>() / days.at(itt.first).size());
            (*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["packets"]["out"] = ((*reply)["data"]["stats"][(*reply)["data"]["stats"].size() - 1]["packets"]["out"].get<int>() / days.at(itt.first).size());
        }
    }
    catch (std::exception& e)
    {
        (*reply)["route"] = (*requestorData)["route"];
        (*reply)["type"] = "GET";
        (*reply)["uid"] = (*requestorData)["uid"];
        (*reply)["error"] = { {"unknown", true }};
    }

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

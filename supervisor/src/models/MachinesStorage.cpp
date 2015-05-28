#include "MachinesStorage.hpp"

#include <exception>
#include <iostream>

#include "../utils/JSON.hpp"

using tin::models::MachinesStorage;
using tin::utils::Machine;

MachinesStorage::MachinesStorage()
{
    this->machinesFileOpen("machines.json");
    this->machinesFileLoadData();
}

int MachinesStorage::newMachine(const std::string& name, const std::string& ip, const unsigned int& port)
{
    Machine m = Machine(ip, name, port);
    m.status = "offline";
    m.lastSynchronization = 0;

    m.addObserver(this);

    this->ipPortIdMap.insert({ std::make_pair(ip, port), m.id });
    this->idMachineMap.insert({ m.id, m });
    this->idMachineMap.at(m.id).updateMagicMembers();

    this->idMachineMap.at(m.id).status = "offline";

    return m.id;
}

int MachinesStorage::newMachine(tin::utils::Machine& machine)
{
    machine.addObserver(this);

    this->ipPortIdMap.insert({ std::make_pair(machine.ip, machine.port), machine.id });
    this->idMachineMap.insert({ machine.id, machine });
    this->idMachineMap.at(machine.id).updateMagicMembers();

    return machine.id;
}

bool MachinesStorage::removeMachine(const int& machineID)
{
    try
    {
        auto& machine = this->idMachineMap.at(machineID);

        machine.removeObserver(this);

        this->ipPortIdMap.erase(std::make_pair(machine.ip, machine.port));
        this->idMachineMap.erase(machineID);

        return true;
    }
    catch (std::exception& e)
    {
        return false;
    }
}

Machine& MachinesStorage::getMachine(const int& machineID)
{
    return this->idMachineMap.at(machineID);
}

Machine& MachinesStorage::getMachine(const std::string& ip, const unsigned int& port)
{
    return this->idMachineMap.at(this->ipPortIdMap.at(std::make_pair(ip, port)));
}

Machine& MachinesStorage::getMachine(const std::string& ip)
{
    int machineID;
    for(auto& it: this->ipPortIdMap)
    {
        if (it.first.first == ip)
        {
            machineID = it.second;
            break;
        }
    }

    return this->getMachine(machineID);
}

bool MachinesStorage::hasMachine(const std::string& ip, const unsigned int& port)
{
    try
    {
        this->idMachineMap.at(this->ipPortIdMap.at(std::make_pair(ip, port)));
        return true;
    }
    catch (std::exception& e)
    {
        return false;
    }
}

bool MachinesStorage::hasMachine(const std::string& ip)
{
    for(auto& it: this->ipPortIdMap)
    {
        if (it.first.first == ip)
        {
            return true;
        }
    }

    return false;
}



void MachinesStorage::machinesFileOpen(const std::string &filename)
{
    std::fstream file;
    file.open(filename);
    this->lastMachinesFile = filename;

    if (!file)
    {
        file.open(filename, std::ios_base::out | std::ios_base::trunc);
        file.close();
        file.open(filename);
    }

    if (!file.is_open())
    {
        std::cout << "[MachinesStorage] Could not open machinesFile (" << filename << ")!!!" << std::endl;
        this->lastMachinesFile = "";
        return;
    }
}

void MachinesStorage::machinesFileLoadData()
{
    std::fstream file;
    file.open(this->lastMachinesFile);

    std::string fileContent;

    file.seekg(0, std::ios::end);
    fileContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    fileContent.assign(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    try
        {
        nlohmann::json loadedJSON = nlohmann::json::parse(fileContent);

        if (!loadedJSON.is_array())
        {
            std::cout << "[MachinesStorage] 'Not an array' in machinesFile (" << this->lastMachinesFile << ")!!!" << std::endl;
            return;
        }

        for(auto& machineJSON: loadedJSON)
        {
            tin::utils::Machine m(machineJSON);
            this->newMachine(m);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "[MachinesStorage] Invalid data in machinesFile (" << this->lastMachinesFile << ")!!!" << std::endl;
    }
}

void MachinesStorage::machinesFileSaveData()
{
    std::fstream file;
    file.open(this->lastMachinesFile, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

    if (!file.is_open())
    {
        std::cout << "[MachinesStorage] Could not reopen machinesFile (" << this->lastMachinesFile << ")!!!" << std::endl;
        return;
    }

    nlohmann::json dataJSON = nlohmann::json::array();
    for(auto& machine: this->idMachineMap)
    {
        dataJSON[dataJSON.size()] = machine.second.serialize();
    }

    file << dataJSON.dump();
}

void MachinesStorage::update()
{
    std::cout << "[MachinesStorage] UPDATING" << std::endl;
    this->machinesFileSaveData();
}

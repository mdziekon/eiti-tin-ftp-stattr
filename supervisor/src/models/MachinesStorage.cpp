#include "MachinesStorage.hpp"

#include <exception>

using tin::models::MachinesStorage;
using tin::utils::Machine;

int MachinesStorage::newMachine(const std::string& name, const std::string& ip, const unsigned int& port)
{
    Machine m = Machine(ip, name, port);
    m.status = "offline";
    m.lastSynchronization = 0;

    this->ipPortIdMap.insert({ std::make_pair(ip, port), m.id });
    this->idMachineMap.insert({ m.id, m });

    return m.id;
}

bool MachinesStorage::removeMachine(const int& machineID)
{
    try
    {
        auto machine = this->idMachineMap.at(machineID);

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

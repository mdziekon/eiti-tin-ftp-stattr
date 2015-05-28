#ifndef TIN_MODELS_MACHINESTORAGE_HPP
#define TIN_MODELS_MACHINESTORAGE_HPP

#include "../utils/Machine.hpp"
#include "../utils/ObserverPattern.hpp"

#include <map>
#include <utility>
#include <fstream>

namespace tin { namespace models {

struct MachinesStorage: public tin::utils::Observer
{
    MachinesStorage();

    int newMachine(const std::string& name, const std::string& ip, const unsigned int& port);
    int newMachine(tin::utils::Machine& machine);
    bool removeMachine(const int& machineID);
    tin::utils::Machine& getMachine(const int& machineID);
    tin::utils::Machine& getMachine(const std::string& ip, const unsigned int& port);
    tin::utils::Machine& getMachine(const std::string& ip);
    bool hasMachine(const std::string& ip, const unsigned int& port);
    bool hasMachine(const std::string& ip);

    void machinesFileOpen(const std::string &filename);
    void machinesFileLoadData();
    void machinesFileSaveData();

    std::string lastMachinesFile;

    std::map<std::pair<std::string, unsigned int>, int> ipPortIdMap;
    std::map<int, tin::utils::Machine> idMachineMap;

private:
    virtual void update();
};

}};

#endif

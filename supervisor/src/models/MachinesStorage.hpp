#ifndef TIN_MODELS_MACHINESTORAGE_HPP
#define TIN_MODELS_MACHINESTORAGE_HPP

#include "../utils/Machine.hpp"

#include <map>
#include <utility>

namespace tin { namespace models {

struct MachinesStorage
{
    std::map<std::pair<std::string, unsigned int>, int> ipPortIdMap;
    std::map<int, tin::utils::Machine> idMachineMap;

    int newMachine(const std::string& name, const std::string& ip, const unsigned int& port);
    bool removeMachine(const int& machineID);
    tin::utils::Machine& getMachine(const int& machineID);
    tin::utils::Machine& getMachine(const std::string& ip, const unsigned int& port);

};

}};

#endif

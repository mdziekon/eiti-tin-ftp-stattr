#ifndef TIN_CONTROLLERS_MAIN_MAINMODULE_HPP
#define TIN_CONTROLLERS_MAIN_MAINMODULE_HPP

#include <vector>
#include <map>
#include <utility>
#include <string>
#include <tuple>

#include "typedefs.hpp"
#include "../terminal/typedefs.hpp"
#include "../../models/typedefs.hpp"
#include "../../utils/typedefs.hpp"
#include "../../network/websocket/typedefs.hpp"
#include "../../network/bsdsocket/typedefs.hpp"

#include "../../utils/QueueThread.hpp"
#include "../../utils/Machine.hpp"
#include "../../utils/JSON.hpp"

#include "Event.hpp"
#include "MainVisitor.hpp"

#include "../../models/MachinesStorage.hpp"

namespace tin { namespace controllers { namespace main
{
    class MainModule: public tin::utils::QueueThread
    <tin::controllers::main::Event, tin::controllers::main::MainVisitor>
    {
        friend class tin::controllers::main::MainVisitor;

        tin::supervisor::models::StatsQueue& statsQueue;
        
    public:
        MainModule(
            tin::controllers::main::ControllerQueue &incomingQueue,
            tin::network::websocket::ManagerQueue &networkManagerQueue,
            tin::network::bsdsocket::ManagerQueue& bsdManagerQueue,
            tin::supervisor::models::StatsQueue& statsQueue,
            tin::controllers::terminal::TerminalQueue& terminalQueue
        );

        tin::models::MachinesStorage machines;
        std::map<std::pair<std::string, unsigned int>, std::tuple<bool, unsigned int, tin::utils::json::ptr>> pingsQueue;
        std::map<std::pair<std::string, unsigned int>, std::tuple<bool, unsigned int, tin::utils::json::ptr>> snifferToggleQueue;
        std::map<std::pair<std::string, unsigned int>, std::tuple<bool, unsigned int, tin::utils::json::ptr>> filterChangeQueue;
        std::map<std::pair<std::string, unsigned int>, std::tuple<bool, unsigned int, tin::utils::json::ptr>> syncQueue;

    private:
        tin::network::websocket::ManagerQueue& networkManagerQueue;
        tin::network::bsdsocket::ManagerQueue& bsdManagerQueue;
        tin::controllers::terminal::TerminalQueue& terminalQueue;
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINMODULE_HPP */

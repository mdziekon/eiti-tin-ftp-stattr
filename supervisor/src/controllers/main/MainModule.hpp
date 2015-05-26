#ifndef TIN_CONTROLLERS_MAIN_MAINMODULE_HPP
#define TIN_CONTROLLERS_MAIN_MAINMODULE_HPP

#include <vector>
#include <map>
#include <utility>
#include <string>

#include "typedefs.hpp"
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

    public:
        MainModule(
            tin::controllers::main::ControllerQueue &incomingQueue,
            tin::network::websocket::ManagerQueue &networkManagerQueue,
            tin::network::bsdsocket::ManagerQueue &bsdQueue
        );

        tin::models::MachinesStorage machines;
        std::map<std::pair<std::string, unsigned int>, std::pair<unsigned int, tin::utils::json::ptr>> pingsQueue;

    private:
        tin::network::websocket::ManagerQueue &networkManagerQueue;
        tin::network::bsdsocket::ManagerQueue &bsdQueue;

    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINMODULE_HPP */

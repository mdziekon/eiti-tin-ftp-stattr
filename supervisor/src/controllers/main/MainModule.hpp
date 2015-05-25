#ifndef TIN_CONTROLLERS_MAIN_MAINMODULE_HPP
#define TIN_CONTROLLERS_MAIN_MAINMODULE_HPP

#include <vector>

#include "typedefs.hpp"
#include "../../models/typedefs.hpp"
#include "../../utils/typedefs.hpp"
#include "../../network/websocket/typedefs.hpp"
#include "../../network/bsdsocket/typedefs.hpp"

#include "../../utils/QueueThread.hpp"

#include "Event.hpp"
#include "MainVisitor.hpp"

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
            tin::supervisor::models::StatsQueue& statsQueue
        );

    private:
        tin::network::websocket::ManagerQueue& networkManagerQueue;
        tin::network::bsdsocket::ManagerQueue& bsdManagerQueue;
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINMODULE_HPP */

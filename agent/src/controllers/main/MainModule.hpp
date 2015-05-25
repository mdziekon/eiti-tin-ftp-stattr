#ifndef TIN_CONTROLLERS_MAIN_MAINMODULE_HPP
#define TIN_CONTROLLERS_MAIN_MAINMODULE_HPP

#include <vector>

#include "typedefs.hpp"
#include "../../utils/typedefs.hpp"
#include "../../network/bsdsocket/typedefs.hpp"
#include "../../models/typedefs.hpp"

#include "../../utils/QueueThread.hpp"

#include "Event.hpp"
#include "MainVisitor.hpp"

namespace tin { namespace controllers { namespace main
{
    class MainModule: public tin::utils::QueueThread
    <tin::controllers::main::Event, tin::controllers::main::MainVisitor>
    {
        friend class tin::controllers::main::MainVisitor;

    public:
        MainModule(
            tin::controllers::main::ControllerQueue &incomingQueue,
            tin::network::bsdsocket::ManagerQueue &networkManagerQueue,
            tin::agent::models::StatsGathererQueue& statsGathererQueue
        );

    private:
        tin::network::bsdsocket::ManagerQueue& networkManagerQueue;
        tin::agent::models::StatsGathererQueue& statsGathererQueue;
    };
}}}

#endif	/* TIN_CONTROLLERS_MAIN_MAINMODULE_HPP */

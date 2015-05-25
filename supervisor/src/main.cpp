#include <iostream>
#include <system_error>

#include "models/Stats.hpp"
#include "controllers/main/MainModule.hpp"
#include "network/websocket/Manager.hpp"
#include "network/bsdsocket/Manager.hpp"

#include "network/bsdsocket/events/MessageRequest.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;
    tin::network::bsdsocket::ManagerQueue bsdManagerQueue;
    tin::supervisor::models::StatsQueue statsQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue, bsdManagerQueue, statsQueue);
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 9001);
    tin::network::bsdsocket::Manager bsdManager(bsdManagerQueue, ctrlQueue);
    tin::supervisor::models::Stats stats(statsQueue, ctrlQueue);

    std::cout << "Hello supervisor!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto bsdManagerThread = bsdManager.createThread();
    auto statsThread = stats.createThread();
    
    stats.updateDataset();

    try
    {
        mainCtrlThread.join();
        netManager.join();
        bsdManagerThread.join();
        statsThread.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

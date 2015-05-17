#include <iostream>
#include <system_error>

#include "controllers/main/MainModule.hpp"
#include "network/bsdsocket/Manager.hpp"
#include "network/sniffer/SnifferManager.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::bsdsocket::ManagerQueue netManagerQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue);
    tin::network::bsdsocket::Manager networkManager(netManagerQueue, ctrlQueue, 3333);
    tin::network::sniffer::SnifferManager sniffManager(ctrlQueue, "lo", "src 127.0.0.1");

    std::cout << "Hello agent!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();

    try
    {
        mainCtrlThread.join();
        netManager.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

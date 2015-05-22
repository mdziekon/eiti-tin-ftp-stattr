#include <iostream>
#include <system_error>

#include "controllers/main/MainModule.hpp"
#include "network/bsdsocket/Manager.hpp"
#include "network/sniffer/Manager.hpp"

#include "network/sniffer/events/ChangeFilter.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::bsdsocket::ManagerQueue netManagerQueue;
    tin::network::sniffer::ManagerQueue snifferManagerQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue, snifferManagerQueue);
    tin::network::bsdsocket::Manager networkManager(netManagerQueue, ctrlQueue, 3333);
    tin::network::sniffer::Manager sniffManager(
        snifferManagerQueue,
        ctrlQueue,
        "lo",
        "src 127.0.0.1"
    );

    std::cout << "Hello agent!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto sniffThread = sniffManager.createThread();

    std::this_thread::sleep_for (std::chrono::seconds(1));

    try
    {
        mainCtrlThread.join();
        netManager.join();
        sniffThread.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

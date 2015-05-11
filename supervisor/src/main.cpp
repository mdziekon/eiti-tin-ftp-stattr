#include <iostream>
#include <system_error>

#include "network/websocket/Manager.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 9001);

    std::cout << "Hello supervisor!\n";

    auto netManager = networkManager.createThread();

    try
    {
        netManager.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

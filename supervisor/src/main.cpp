#include <iostream>
#include <system_error>

#include "network/websocket/Manager.hpp"
#include "network/bsdsocket/Manager.hpp"

#include "network/bsdsocket/events/MessageRequest.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 9001);
    tin::network::bsdsocket::ManagerQueue bsdManagerQueue;
    tin::network::bsdsocket::Manager bsdManager(bsdManagerQueue, ctrlQueue);

    std::cout << "Hello supervisor!\n";

    auto netManager = networkManager.createThread();
    auto bsdManagerThread = bsdManager.createThread();

    bsdManagerQueue.push(
        tin::network::bsdsocket::EventPtr(
            new tin::network::bsdsocket::events::MessageRequest(
                "localhost",
                3333,
                "testmessage",
                false
            )
        )
    );

    try
    {
        netManager.join();
        bsdManagerThread.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

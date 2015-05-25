#include <iostream>
#include <system_error>

#include "controllers/main/MainModule.hpp"
#include "network/bsdsocket/Manager.hpp"
#include "network/sniffer/Manager.hpp"
#include "models/StatsGatherer.hpp"

#include "network/sniffer/events/ChangeFilter.hpp"
#include "network/bsdsocket/events/MessageReceived.hpp"
#include "models/events/IncomingPacket.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::bsdsocket::ManagerQueue netManagerQueue;
    tin::network::sniffer::ManagerQueue snifferManagerQueue;
    tin::agent::models::StatsGathererQueue statsGathererQueue;

    tin::controllers::main::MainModule mainCtrl(
        ctrlQueue, 
        netManagerQueue,
        statsGathererQueue,
        snifferManagerQueue
    );
    tin::network::bsdsocket::Manager networkManager(netManagerQueue, ctrlQueue, 3333);
    tin::network::sniffer::Manager sniffManager(
        snifferManagerQueue,
        ctrlQueue,
        "lo",
        "src 127.0.0.1"
    );
    tin::agent::models::StatsGatherer statsGatherer(
        statsGathererQueue,
        ctrlQueue
    );

    std::cout << "Hello agent!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto sniffThread = sniffManager.createThread();
    auto statsThread = statsGatherer.createThread();

    std::this_thread::sleep_for (std::chrono::seconds(1));
    
    /*
    
    snifferManagerQueue.push(
        std::make_shared<tin::network::sniffer::events::ChangeFilter>(
            "lo",
            "src 127.0.0.1"
        )
    );
    
    */
    
    try
    {
        statsThread.join();
        sniffThread.join();
        mainCtrlThread.join();
        netManager.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }
    
    return 0;
}

#include <iostream>
#include <system_error>

#include "controllers/main/MainModule.hpp"
#include "controllers/terminal/TerminalModule.hpp"
#include "network/websocket/Manager.hpp"
#include "network/bsdsocket/Manager.hpp"

#include "network/bsdsocket/events/MessageRequest.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;
    tin::controllers::terminal::TerminalQueue terminalQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue);
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 9001);
    tin::network::bsdsocket::ManagerQueue bsdManagerQueue;
    tin::network::bsdsocket::Manager bsdManager(bsdManagerQueue, ctrlQueue);

    // Terminal Server
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread terminalServiceThread([&io_service]() { io_service.run(); });
    tin::controllers::terminal::TerminalModule terminalCtrl(terminalQueue, ctrlQueue, io_service);

    std::cout << "Hello supervisor!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto bsdManagerThread = bsdManager.createThread();
    auto terminalCtrlThread = terminalCtrl.createThread();

    bsdManagerQueue.push(
        tin::network::bsdsocket::EventPtr(
            new tin::network::bsdsocket::events::MessageRequest(
                "localhost",
                3333,
                tin::utils::json::makeSharedInstance("{ \"command\": \"test\" }"),
                true
            )
        )
    );

    try
    {
        mainCtrlThread.join();
        netManager.join();
        bsdManagerThread.join();
        terminalCtrlThread.join();
        terminalServiceThread.join();

    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

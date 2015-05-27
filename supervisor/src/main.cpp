#include <iostream>
#include <system_error>

#include "models/Stats.hpp"
#include "controllers/main/MainModule.hpp"
#include "controllers/terminal/TerminalModule.hpp"
#include "network/websocket/Manager.hpp"
#include "network/bsdsocket/Manager.hpp"

#include "network/bsdsocket/events/MessageRequest.hpp"
#include "network/websocket/events/MessageReceived.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;
    tin::network::bsdsocket::ManagerQueue bsdManagerQueue;
    tin::supervisor::models::StatsQueue statsQueue;
    tin::controllers::terminal::TerminalQueue terminalQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue, bsdManagerQueue, statsQueue, terminalQueue);
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 3338);
    tin::network::bsdsocket::Manager bsdManager(bsdManagerQueue, ctrlQueue);
    tin::supervisor::models::Stats stats(statsQueue, ctrlQueue, mainCtrl.machines);

    // Terminal Server
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread terminalServiceThread([&io_service]() { io_service.run(); });
    tin::controllers::terminal::TerminalModule terminalCtrl(terminalQueue, ctrlQueue, io_service);

    std::cout << "Hello supervisor!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto terminalCtrlThread = terminalCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto bsdManagerThread = bsdManager.createThread();
    auto statsThread = stats.createThread();
    // auto statsRequestorThread = stats.createRequestorThread(2500, ctrlQueue);

    // netManagerQueue.push(std::make_shared<tin::network::websocket::events::MessageReceived>(1, "{\"route\":\"stats-per-day\",\"type\":\"POST\",\"uid\":1}"));

    try
    {
        /*
        
        while (true)
        {
            std::this_thread::sleep_for (std::chrono::seconds(1));

            nlohmann::json msgJson = nlohmann::json::object();
            std::string data;

            std::cout << "[Supervisor] Type command to send:" << std::endl;
            std::getline(std::cin, data);

            msgJson["cmd"] = data;

            while (true)
            {
                std::string key;

                std::cout << "? Pass next argument [Y/N] " << std::endl;
                std::getline(std::cin, data);

                if (data != "Y" && data != "y")
                {
                    break;
                }

                std::cout << "? Key " << std::endl;
                std::getline(std::cin, key);

                std::cout << "? Type [string/number/bool] " << std::endl;
                std::getline(std::cin, data);

                if (data == "string")
                {
                    std::cout << "? Value (string) " << std::endl;
                    std::getline(std::cin, data);

                    msgJson[key] = data;
                }
                else if (data == "number")
                {
                    std::cout << "? Value (number) " << std::endl;
                    std::getline(std::cin, data);

                    msgJson[key] = std::stoi(data);
                }
                else if (data == "bool")
                {
                    std::cout << "? Value (bool) [true/false] " << std::endl;
                    std::getline(std::cin, data);

                    if (data == "true")
                    {
                        msgJson[key] = true;
                    }
                    else
                    {
                        msgJson[key] = false;
                    }
                }
                else
                {
                    std::cout << "! Invalid input " << std::endl;
                }
            }

            bsdManagerQueue.push(
                std::make_shared<tin::network::bsdsocket::events::MessageRequest>(
                    "localhost",
                    3333,
                    std::make_shared<nlohmann::json>(msgJson),
                    true
                )
            );
        }
        
        */
        
        mainCtrlThread.join();
        netManager.join();
        bsdManagerThread.join();
        statsThread.join();
        terminalCtrlThread.join();
        terminalServiceThread.join();
        // statsRequestorThread.join();
    }
    catch (std::system_error& e)
    {
        std::cout << e.what();
    }

    return 0;
}

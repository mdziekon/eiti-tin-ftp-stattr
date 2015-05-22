#include <iostream>
#include <system_error>

#include "controllers/main/MainModule.hpp"
#include "network/websocket/Manager.hpp"
#include "network/bsdsocket/Manager.hpp"

#include "network/bsdsocket/events/MessageRequest.hpp"

int main()
{
    tin::controllers::main::ControllerQueue ctrlQueue;
    tin::network::websocket::ManagerQueue netManagerQueue;

    tin::controllers::main::MainModule mainCtrl(ctrlQueue, netManagerQueue);
    tin::network::websocket::Manager networkManager(netManagerQueue, ctrlQueue, 3338);
    tin::network::bsdsocket::ManagerQueue bsdManagerQueue;
    tin::network::bsdsocket::Manager bsdManager(bsdManagerQueue, ctrlQueue);

    std::cout << "Hello supervisor!\n";

    auto mainCtrlThread = mainCtrl.createThread();
    auto netManager = networkManager.createThread();
    auto bsdManagerThread = bsdManager.createThread();

    // bsdManagerQueue.push(
    //     tin::network::bsdsocket::EventPtr(
    //         new tin::network::bsdsocket::events::MessageRequest(
    //             "localhost",
    //             3333,
    //             tin::utils::json::makeSharedInstance("{ \"cmd\": \"sync\" }"),
    //             true
    //         )
    //     )
    // );

    try
    {
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

        mainCtrlThread.join();
        netManager.join();
        bsdManagerThread.join();
    }
    catch (std::system_error& e)
    {
        // Could not join one of the threads
    }

    return 0;
}

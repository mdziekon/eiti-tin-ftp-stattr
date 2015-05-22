#include "MainVisitor.hpp"

#include <iostream>
#include <string.h>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdReceived.hpp"

#include "../../network/bsdsocket/events/ResponseRequest.hpp"
#include "../../network/bsdsocket/events/ConnectionTerminationRequest.hpp"
#include "../../network/sniffer/events/ChangeFilter.hpp"

namespace events = tin::controllers::main::events;
namespace bsdsocketEvents = tin::network::bsdsocket::events;
namespace snifferEvents = tin::network::sniffer::events;

using nlohmann::json;

tin::controllers::main::MainVisitor::MainVisitor(tin::controllers::main::MainModule& controller):
controller(controller)
{}

void tin::controllers::main::MainVisitor::visit(events::Terminate &event)
{
    this->controller.terminate();
}

void tin::controllers::main::MainVisitor::visit(events::CmdReceived &event)
{
    std::cout << "[MainCtrl] Command Received, processing" << std::endl;

	json& temp = *(event.jsonPtr);

    if (temp.find("cmd") == temp.end() || !temp["cmd"].is_string())
    {
        std::cout << "[MainCtrl] Invalid command received" << std::endl;
        return;
    }

    const std::string cmd = temp["cmd"];

    if (cmd == "sync")
    {
        this->controller.networkManagerQueue.push(
            std::make_shared<bsdsocketEvents::ResponseRequest>(
                std::make_shared<json>(
                    json::parse("{ \"testMessage\": \"testResponse\", \"testTable\": { \"testArray\": [ 1, 2, 3 ], \"test\": true } }")
                )
            )
        );
    }
    else if (cmd == "change_filter")
    {
        if (temp.find("device") == temp.end() || temp.find("expression") == temp.end() || !temp["device"].is_string() || !temp["expression"].is_string())
        {
            std::cout << "[MainCtrl] Invalid arguments for \"change_filter\" command" << std::endl;

            this->controller.networkManagerQueue.push(
                std::make_shared<bsdsocketEvents::ResponseRequest>(
                    std::make_shared<json>(
                        json::parse("{ \"error\": { \"invalid\": [ \"device\", \"expression\" ] } }")
                    )
                )
            );

            return;
        }

        std::cout << "[MainCtrl] Received change_filter, changing to: " << temp["device"] << " / " << temp["expression"] << std::endl;

        this->controller.snifferManagerQueue.push(
            std::make_shared<snifferEvents::ChangeFilter>(
                temp["device"], temp["expression"]
            )
        );

        this->controller.networkManagerQueue.push(
            std::make_shared<bsdsocketEvents::ResponseRequest>(
                std::make_shared<json>(
                    json::parse("{ \"cmd\": \"change_filter\", \"success\": true }")
                )
            )
        );
    }
    else
    {
        std::cout << "[MainCtrl] Invalid command received: " << cmd << std::endl;

        this->controller.networkManagerQueue.push(
            std::make_shared<bsdsocketEvents::ResponseRequest>(
                std::make_shared<json>(
                    json::parse("{ \"error\": { \"invalid\": \"cmd\" } }")
                )
            )
        );
    }
}

void tin::controllers::main::MainVisitor::visit(tin::controllers::main::events::PacketReceived &event)
{

}


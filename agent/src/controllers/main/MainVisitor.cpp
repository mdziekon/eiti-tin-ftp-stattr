#include "MainVisitor.hpp"

#include <iostream>
#include <string.h>

#include "MainModule.hpp"

#include "events/Terminate.hpp"
#include "events/CmdReceived.hpp"
#include "events/SnifferStatus.hpp"

#include "../../network/bsdsocket/events/ResponseRequest.hpp"
#include "../../network/bsdsocket/events/ConnectionTerminationRequest.hpp"
#include "../../network/sniffer/events/ChangeFilter.hpp"
#include "../../network/sniffer/events/StartSniffing.hpp"
#include "../../network/sniffer/events/StopSniffing.hpp"
#include "../../network/sniffer/events/IsSniffing.hpp"

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

    else if (cmd == "ping")
    {
        this->controller.lastCMD = "ping";

        this->controller.snifferManagerQueue.push(
            std::make_shared<snifferEvents::IsSniffing>()
        );
    }

    else if (cmd == "startsniffing")
    {
        std::cout << "[MainCtrl] Received startsniffing" << std::endl;

        this->controller.lastCMD = "startsniffing";

        this->controller.snifferManagerQueue.push(
            std::make_shared<snifferEvents::StartSniffing>()
        );
    }

    else if (cmd == "stopsniffing")
    {
        std::cout << "[MainCtrl] Received startsniffing" << std::endl;

        this->controller.lastCMD = "stopsniffing";

        this->controller.snifferManagerQueue.push(
            std::make_shared<snifferEvents::StopSniffing>()
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

        this->controller.lastCMD = "change_filter";

        this->controller.snifferManagerQueue.push(
            std::make_shared<snifferEvents::ChangeFilter>(
                temp["device"], temp["expression"]
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

void tin::controllers::main::MainVisitor::visit(tin::controllers::main::events::SnifferStatus &event)
{
    std::string response;

    if (this->controller.lastCMD == "ping")
    {
        if (event.is)
        {
            response = "{ \"cmd\": \"ping\", \"response\": \"sniffing\" }";
        }
        else
        {
            response = "{ \"cmd\": \"ping\", \"response\": \"stand-by\" }";
        }
    }
    else if (this->controller.lastCMD == "startsniffing")
    {
        if (event.is)
        {
            response = "{ \"cmd\": \"startsniffing\", \"success\": true }";
        }
        else
        {
            response = "{ \"cmd\": \"startsniffing\", \"error\": true }";
        }
    }
    else if (this->controller.lastCMD == "stopsniffing")
    {
        if (!event.is)
        {
            response = "{ \"cmd\": \"stopsniffing\", \"success\": true }";
        }
        else
        {
            response = "{ \"cmd\": \"stopsniffing\", \"error\": true }";
        }
    }
    else if (this->controller.lastCMD == "change_filter")
    {
        if (event.is)
        {
            response = "{ \"cmd\": \"change_filter\", \"success\": true, \"data\": { \"status\": \"sniffing\" } }";
        }
        else
        {
            response = "{ \"cmd\": \"change_filter\", \"success\": true, \"data\": { \"status\": \"stand-by\" } }";
        }
    }

    this->controller.networkManagerQueue.push(
        std::make_shared<bsdsocketEvents::ResponseRequest>(
            std::make_shared<json>(json::parse(response))
        )
    );

}

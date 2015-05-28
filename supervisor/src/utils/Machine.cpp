#include "Machine.hpp"

tin::utils::Machine::Machine()
{
    this->updateMagicMembers();
}

void tin::utils::Machine::updateMagicMembers()
{
    this->id.machine = this;
    this->name.machine = this;
    this->ip.machine = this;
    this->port.machine = this;
    this->filterDevice.machine = this;
    this->filterExpression.machine = this;
    this->status.machine = this;
    this->lastSynchronization.machine = this;
}

tin::utils::Machine::Machine(const nlohmann::json& jsonObj):
Machine()
{
    this->id = incCounter();
    this->name = jsonObj["name"].get<std::string>();
    this->ip = jsonObj["ip"].get<std::string>();
    this->port = jsonObj["port"].get<unsigned int>();
    this->filterDevice = jsonObj["filterDevice"].get<std::string>();
    this->filterExpression = jsonObj["filterExpression"].get<std::string>();
    this->status = jsonObj["status"].get<std::string>();
    this->lastSynchronization = jsonObj["lastSynchronization"].get<int>();
}



tin::utils::Machine::Machine(std::string ip, std::string name,  unsigned int port):
Machine()
{
	this->id = incCounter();
	this->ip = ip;
    this->name = name;
	this->port = port;
};

tin::utils::Machine::Machine(
    const int &id,
    const std::string &ip,
    const unsigned int &port
):
Machine()
{
    this->id = id;
    this->ip = ip;
    this->port = port;
};

int tin::utils::Machine::incCounter()
{
	static int counter = 0;
	return counter++;
}

nlohmann::json tin::utils::Machine::serialize()
{
    nlohmann::json json = {
        { "name", this->name() },
        { "ip", this->ip() },
        { "port", this->port() },
        { "filterDevice", this->filterDevice() },
        { "filterExpression", this->filterExpression() },
        { "status", this->status() },
        { "lastSynchronization", this->lastSynchronization() }
    };

    return json;
}

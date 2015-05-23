#include "Packet.hpp"

#include <iostream>

using Packet = tin::utils::Packet;

Packet::Packet(
    const unsigned long long &no,
    const time_t &timestamp,
    const struct in_addr &sourceIP,
    const struct in_addr &destinationIP,
    const uint8_t &protocol,
    const uint16_t &sourcePort,
    const uint16_t &destinationPort,
    const int &payloadSize
):
sourceIP(sourceIP),
destinationIP(destinationIP),
no(no),
timestamp(timestamp),
protocol(protocol),
sourcePort(ntohs(sourcePort)),
destinationPort(ntohs(destinationPort)),
payloadSize(payloadSize)
{}

Packet::Packet(
    const unsigned long long &no,
    const time_t &timestamp,
    const struct in_addr &sourceIP,
    const struct in_addr &destinationIP,
    const uint8_t &protocol
):
Packet(no, timestamp, sourceIP, destinationIP, protocol, 0, 0, -1)
{}

const std::string Packet::getSourceIP() const
{
    return std::string(inet_ntoa(this->sourceIP));
}

const std::string Packet::getDestinationIP() const
{
    return std::string(inet_ntoa(this->destinationIP));
}

void Packet::printData() const
{
    std::cout << std::endl;
    std::cout << "   Packet no: " << this->no << std::endl
              << "   Timestamp: " << this->timestamp << std::endl
              << "        From: " << this->getSourceIP() << ":" << this->sourcePort << std::endl
              << "          To: " << this->getDestinationIP() << ":" << this->destinationPort << std::endl;

    switch(this->protocol)
    {
        case IPPROTO_TCP:
            std::cout << "    Protocol: TCP" << std::endl;
            break;
        case IPPROTO_UDP:
            std::cout << "    Protocol: UDP" << std::endl;
            return;
        case IPPROTO_ICMP:
            std::cout << "    Protocol: ICMP" << std::endl;
            return;
        case IPPROTO_IP:
            std::cout << "    Protocol: IP" << std::endl;
            return;
        default:
            std::cout << "    Protocol: unknown" << std::endl;
            return;
    }

    if (this->payloadSize > 0)
    {
        std::cout << "Payload size: " << this->payloadSize << " bytes" << std::endl;
    }

    return;
}

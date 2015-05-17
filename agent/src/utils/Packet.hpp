#ifndef TIN_UTILS_PACKET_HPP
#define TIN_UTILS_PACKET_HPP

#include <memory>

#include <cstdint>
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tin { namespace utils
{
    class Packet
    {
        const struct in_addr sourceIP;
        const struct in_addr destinationIP;

    public:
        typedef std::shared_ptr<Packet> ptr;

        const unsigned long long no;
        const int timestamp;
        const uint8_t protocol;
        const uint16_t sourcePort;
        const uint16_t destinationPort;
        const int payloadSize;

        Packet(
            const unsigned long long &no,
            const time_t &timestamp,
            const struct in_addr &sourceIP,
            const struct in_addr &destinationIP,
            const uint8_t &protocol,
            const uint16_t &sourcePort,
            const uint16_t &destinationPort,
            const int &payloadSize
        );
        Packet(
            const unsigned long long &no,
            const time_t &timestamp,
            const struct in_addr &sourceIP,
            const struct in_addr &destinationIP,
            const uint8_t &protocol
        );

        const std::string getSourceIP() const;
        const std::string getDestinationIP() const;
        void printData() const;
    };
}}

#endif /* TIN_UTILS_PACKET_HPP */

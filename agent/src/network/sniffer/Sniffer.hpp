#ifndef TIN_NETWORK_SNIFFER_SNIFFER_HPP
#define TIN_NETWORK_SNIFFER_SNIFFER_HPP

#include <pcap.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <string>
#include <thread>
#include <functional>
#include <memory>

#include "../../utils/Packet.hpp"
#include "../../utils/HandlersContainer.hpp"

namespace tin { namespace network { namespace sniffer
{
    // Max Packet Size
    #define SNAP_LEN 1518

    // Size of Ethernet header
    #define SIZE_ETHERNET 14

    // Ethernet address = 6 bytes
    #define ETHER_ADDR_LEN  6

    // Ethernet header
    typedef struct ethernet_header
    {
            u_char  ethernet_dhost[ETHER_ADDR_LEN];    // destination
            u_char  ethernet_shost[ETHER_ADDR_LEN];    // source
            u_short ethernet_type;                     // type
    } eth_t;

    /* IP header */
    typedef struct ip_header
    {
            u_char  ip_vhl;                 // version
            u_char  ip_tos;                 // type of service
            u_short ip_len;                 // total length
            u_short ip_id;                  // id
            u_short ip_offset;              // offset
            #define IP_RF 0x8000            // reserved fragment flag
            #define IP_DF 0x4000            // dont fragment flag
            #define IP_MF 0x2000            // more fragments flag
            #define IP_OFFMASK 0x1fff       // mask for fragmenting bits
            u_char  ip_ttl;                 // time to live
            u_char  ip_pro;                 // protocol
            u_short ip_checksum;            // checksum
            struct  in_addr ip_src, ip_dst; // source and dest address
    } ip_t;

    #define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
    #define IP_V(ip)                (((ip)->ip_vhl) >> 4)

    // TCP Header
    typedef u_int tcp_seq;

typedef struct tcp_header
{
        u_short th_sport;               // source port
        u_short th_dport;               // destination port
        tcp_seq th_seq;                 // sequence number
        tcp_seq th_ack;                 // acknowledgement number
        u_char  th_offx2;               // data offset
        #define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 // window
        u_short th_sum;                 // checksum
        u_short th_urp;                 // urgent pointer
} tcp_t;

    typedef struct pcaprec_header
    {
            u_int32_t ts_sec;         // timestamp seconds
            u_int32_t ts_usec;        // timestamp microseconds
            u_int32_t incl_len;       // number of octets of packet saved in file
            u_int32_t orig_len;       // actual length of packet
    } pcaprec_t;

    class Sniffer
    {
    public:
        Sniffer(const std::string& device, const std::string& filter);

        void sniff();

        void handlePacket(const struct pcap_pkthdr *header, const u_char *packet);
        void run();
        unsigned int attachPacketReceivedHandler(std::function<void(const tin::utils::Packet::ptr&)>& handler);
        unsigned int attachPacketReceivedHandler(std::function<void(const tin::utils::Packet::ptr&)>&& handler);

    private:
        std::string device;
        std::string expression;

        unsigned long long packetCounter = 0;

        std::thread snifferThread;

        tin::utils::HandlersContainer<void(const tin::utils::Packet::ptr&)> packetReceivedHandlers;
        void runPacketReceivedHandlers(const tin::utils::Packet::ptr& packetPtr);
    };
}}}

#endif  /* TIN_NETWORK_SNIFFER_SNIFFER_HPP */

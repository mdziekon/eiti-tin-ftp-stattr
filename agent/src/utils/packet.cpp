#include "packet.h"

tin::utils::Packet::Packet(
    int packetNumber, 
    time_t timestamp, 
    struct in_addr ip_src, 
    struct in_addr ip_dst, 
    u_char ip_protocol
):
packetNumber(packetNumber),
timestamp(timestamp),
ip_src(ip_src),
ip_dst(ip_dst),
ip_protocol(ip_protocol),
th_sport(0),
th_dport(0),
size_payload(-1)
{}

tin::utils::Packet::Packet(
    int packetNumber, 
    time_t timestamp, 
    struct in_addr ip_src, 
    struct in_addr ip_dst, 
    u_char ip_protocol, 
    u_short th_sport, 
    u_short th_dport, 
    int size_payload
):
packetNumber(packetNumber),
timestamp(timestamp),
ip_src(ip_src),
ip_dst(ip_dst),
ip_protocol(ip_protocol),
th_sport(th_sport),
th_dport(th_dport),
size_payload(size_payload)
{}

tin::utils::Packet::Packet(const Packet &pac):
packetNumber(pac.packetNumber),
timestamp(pac.timestamp),
ip_src(pac.ip_src),
ip_dst(pac.ip_dst),
ip_protocol(pac.ip_protocol),
th_sport(pac.th_sport),
th_dport(pac.th_dport),
size_payload(pac.size_payload)
{}

tin::utils::Packet::Packet(const Packet *packet):
packetNumber(packet->packetNumber),
timestamp(packet->timestamp),
ip_src(packet->ip_src),
ip_dst(packet->ip_dst),
ip_protocol(packet->ip_protocol),
th_sport(packet->th_sport),
th_dport(packet->th_dport),
size_payload(packet->size_payload)
{}

void tin::utils::Packet::showPacketInfo()
{
    printf("\nPacket number %ld:\n", packetNumber);
    printf("  Timestamp: %d\n", timestamp);
    printf("       From: %s\n", inet_ntoa(ip_src));
    printf("         To: %s\n", inet_ntoa(ip_dst));


    switch(ip_protocol) 
    {
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            break;
        case IPPROTO_UDP:
            printf("   Protocol: UDP\n");
            return;
        case IPPROTO_ICMP:
            printf("   Protocol: ICMP\n");
            return;
        case IPPROTO_IP:
            printf("   Protocol: IP\n");
            return;
        default:
            printf("   Protocol: unknown\n");
            return;
    }

    printf("   Src port: %d\n", ntohs(th_sport));
    printf("   Dst port: %d\n", ntohs(th_dport));

    if (size_payload > 0) 
    {
        printf("   Size of Payload : (%d bytes)\n", size_payload);

    }

    return;
}

tin::utils::Packet::ptr tin::utils::Packet::makeSharedInstance(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol)
{
    return tin::utils::Packet::ptr(new tin::utils::Packet(packetNumber, timestamp, ip_src, ip_dst,  ip_protocol));
}

tin::utils::Packet::ptr tin::utils::Packet::makeSharedInstance(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol, u_short th_sport, u_short th_dport, int size_payload)
{
    return tin::utils::Packet::ptr(new tin::utils::Packet(packetNumber, timestamp, ip_src, ip_dst, ip_protocol, th_sport, th_dport, size_payload));
}

tin::utils::Packet::ptr tin::utils::Packet::getSharedInstance(const tin::utils::Packet& pac)
{
    return tin::utils::Packet::ptr(new tin::utils::Packet(pac));
}
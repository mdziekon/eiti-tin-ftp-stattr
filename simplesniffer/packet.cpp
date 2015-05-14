#include "packet.h"

Packet::Packet(int packetNumber, int timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol)
{
	this->packetNumber = packetNumber;
	this->timestamp = timestamp;
	this->ip_src = ip_src;
	this->ip_dst = ip_dst;
	this->ip_protocol = ip_protocol;
	this->th_sport = 0; // unknown
	this->th_dport = 0; // unknown
	this->size_payload = -1; // unknown
}

Packet::Packet(int packetNumber, int timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol, u_short th_sport, u_short th_dport, int size_payload)
{
	this->packetNumber = packetNumber;
	this->timestamp = timestamp;
	this->ip_src = ip_src;
	this->ip_dst = ip_dst;
	this->ip_protocol = ip_protocol;
	this->th_sport = th_sport;
	this->th_dport = th_dport;
	this->size_payload = size_payload;
}

void Packet::showPacketInfo()
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
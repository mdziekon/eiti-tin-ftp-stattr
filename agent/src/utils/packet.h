#ifndef TIN_UTILS_PACKET_H
#define TIN_UTILS_PACKET_H

#include <stdlib.h>

#include <iostream>
#include <memory>

#include <cstdio>
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tin { namespace utils {

class Packet
{
public:
	Packet(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol);
	Packet(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol, u_short th_sport, u_short th_dport, int size_payload);
	Packet(const Packet& packet);
	Packet(const Packet *packet);
	void showPacketInfo();
	typedef std::shared_ptr<tin::utils::Packet> ptr;
	ptr makeSharedInstance(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol);
	ptr makeSharedInstance(int packetNumber, time_t timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol, u_short th_sport, u_short th_dport, int size_payload);
    ptr getSharedInstance(const tin::utils::Packet& packet);



private:
	long int packetNumber; // Packet Number
	int timestamp; // Timestamp
	struct  in_addr ip_src, ip_dst; // ip src, ip dest
	u_char ip_protocol; // protocol 
	u_short th_sport;   // source port 
    u_short th_dport;   // destination port
	int size_payload; // size of packet


};

} }
#endif /* TIN_UTILS_PACKET_H */
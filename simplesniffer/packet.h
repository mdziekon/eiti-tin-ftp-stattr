#include <stdlib.h>
#include <iostream>
#include <netinet/in.h>
#include <cstdio>
#include <arpa/inet.h>

class Packet
{
public:
	Packet(int packetNumber, int timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol);
	Packet(int packetNumber, int timestamp, struct in_addr ip_src, struct in_addr ip_dst, u_char ip_protocol, u_short th_sport, u_short th_dport, int size_payload);
	void showPacketInfo();

	long int packetNumber; // Packet Number
	int timestamp; // Timestamp
	struct  in_addr ip_src, ip_dst; // ip src, ip dest
	u_char ip_protocol; // protocol 
	u_short th_sport;   // source port 
    u_short th_dport;   // destination port
	int size_payload; // size of packet


};
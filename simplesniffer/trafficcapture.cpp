#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <ctime>
#include "packet.h"
#include <vector>

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

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
char* getExpression();
char* getDevice();

std::vector<Packet*> packetVector;

int main(int argc, char **argv)
{

    char *dev = NULL;              // capture device name 
    char errbuf[PCAP_ERRBUF_SIZE]; // error buffer 
    pcap_t *handle;                // packet capture handle 

    char *filter = NULL;             // filter expression
    struct bpf_program fp;         // compiled filter program (expression) 
    bpf_u_int32 mask;              // subnet mask 
    bpf_u_int32 net;               // ip 
    int num_packets = -1;          // number of packets to capture (-1 means till error) */


    // get device
    dev = getDevice();


    if (dev == NULL) 
    {
        fprintf(stderr, "Couldn't find default device: %s\n",
            errbuf);
        exit(EXIT_FAILURE);
    }
    
    
    // get network number and mask associated with capture device 
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) 
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
            dev, errbuf);
        net = 0;
        mask = 0;
    }

    // get an expression
    filter = getExpression();

    // print info 
    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter);

    // open capture device
    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }

    // make sure we're capturing on an Ethernet device [2]
    if (pcap_datalink(handle) != DLT_EN10MB) 
    {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
        exit(EXIT_FAILURE);
    }

    // compile the filter 
    if (pcap_compile(handle, &fp, filter, 0, net) == -1) 
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
            filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // apply the filter 
    if (pcap_setfilter(handle, &fp) == -1) 
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
            filter, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // capturing packets
    pcap_loop(handle, num_packets, got_packet, NULL);

    // cleanup 
    pcap_freecode(&fp);
    pcap_close(handle);

    printf("\nCapture complete.\n");

return 0;
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

    static int counter = 0;                   // packet counter 
    
    // declare pointers to packet headers 
    const struct ethernet_header *ethernet;      // The ethernet header [1] 
    const struct ip_header *ip;             // The IP header 
    const struct tcp_header *tcp;           // The TCP header 
    const struct pcaprec_header *pcap;      // The PCAP record header

    int size_ip;
    int size_tcp;
    int size_payload;
    
    counter++;
    
    // define ethernet header
    ethernet = (struct ethernet_header*)(packet);
    pcap = (struct pcaprec_header*)(packet);

    long date = (long) pcap->ts_sec;
    time_t timestamp = time(&date);
    
    // define/compute ip header offset 
    ip = (struct ip_header*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) 
    {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    // if it is not TCP 
    if(ip->ip_pro != IPPROTO_TCP)
    {
        Packet *pac = new Packet(counter, timestamp, ip->ip_src, ip->ip_dst, ip->ip_pro);
        packetVector.push_back(pac);
        pac->showPacketInfo();
        return;
    }
    
    // If it is TCP - we have some extra knowledge
    
    // Tcp header offset
    tcp = (struct tcp_header*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if (size_tcp < 20) 
    {
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
        return;
    } 
    
    // compute tcp payload (segment) size 
    size_payload = ntohs(ip->ip_len) + (size_ip + size_tcp);
    
    // create new "Packet" and add it to vector
    Packet *pac = new Packet(counter, timestamp, ip->ip_src, ip->ip_dst, ip->ip_pro, tcp->th_sport, tcp->th_dport, size_payload);
    packetVector.push_back(pac);
    pac->showPacketInfo();

return;
}

char* getDevice()
{
   std::string str;
   char* dev;
   printf(">Give Device Name\n");
   std::getline(std::cin, str);
   dev = strdup(str.c_str());
   return dev;
}

char* getExpression()
{
   std::string str;
   char *expression;
   printf(">Give Expression\n");
   std::getline(std::cin, str);
   expression = strdup(str.c_str());
   return expression;
}


#include "Sniffer.hpp"

using namespace tin::network::trafficcapture;

std::vector<std::shared_ptr<tin::utils::Packet>> SnifferProxy::packetVector;
std::mutex SnifferProxy::packetMutex;
std::condition_variable SnifferProxy::packetVectorEmpty;

void SnifferProxy::gotPacket(u_char *user, const struct pcap_pkthdr *header, const u_char *packet)
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
        auto pac = std::make_shared<tin::utils::Packet>(new tin::utils::Packet(counter, timestamp, ip->ip_src, ip->ip_dst, ip->ip_pro));
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
    auto pac = std::make_shared<tin::utils::Packet>(new tin::utils::Packet(counter, timestamp, ip->ip_src, ip->ip_dst, ip->ip_pro, tcp->th_sport, tcp->th_dport, size_payload));
    SnifferProxy::pushPacket(pac);
    pac->showPacketInfo();
}

void SnifferProxy::pushPacket(const std::shared_ptr<tin::utils::Packet>& packet)
{
    std::unique_lock<std::mutex> lock(SnifferProxy::packetMutex);
    SnifferProxy::packetVector.push_back(packet);
    if(packetVector.size() == 1) {
        SnifferProxy::packetVectorEmpty.notify_all();
    }
}

std::shared_ptr< tin::utils::Packet > SnifferProxy::pollPacket()
{
    std::unique_lock<std::mutex> lock(SnifferProxy::packetMutex);
    SnifferProxy::packetVectorEmpty.wait(lock);
    auto packet = SnifferProxy::packetVector.back();
    SnifferProxy::packetVector.pop_back();
    return packet;
}

Sniffer::Sniffer(const std::string& device, const std::string& expression):
device(device),
expression(expression)
{}

void Sniffer::sniff()
{
    char errbuf[PCAP_ERRBUF_SIZE]; // error buffer 
    pcap_t *handle;                // packet capture handle 

    struct bpf_program fp;         // compiled filter program (expression) 
    bpf_u_int32 mask;              // subnet mask 
    bpf_u_int32 net;               // ip 
    int num_packets = -1;          // number of packets to capture (-1 means till error) */


    // get device
    auto dev = getDevice(device);


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
    filter = getExpression(expression);

    // print info 
    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter.c_str());

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
    if (pcap_compile(handle, &fp, filter.c_str(), 0, net) == -1) 
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
            filter.c_str(), pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // apply the filter 
    if (pcap_setfilter(handle, &fp) == -1) 
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
            filter.c_str(), pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // capturing packets
    pcap_loop(handle, num_packets, SnifferProxy::gotPacket, NULL);

    // cleanup 
    pcap_freecode(&fp);
    pcap_close(handle);

    printf("\nCapture complete.\n");

}



const char* Sniffer::getDevice(const std::string& device)
{
   char* dev;
   dev = strdup(device.c_str());
   return dev;
}

const char* Sniffer::getExpression(const std::string& expression)
{
   this->expression = strdup(device.c_str());
   return expression.c_str();
}

void Sniffer::run()
{
	this->snifferThread = std::thread(
        &Sniffer::sniff,
        std::ref(*this)
    );

    this->snifferThread.detach();
}

unsigned int Sniffer::attachPacketReceivedHandler(std::function<void(const tin::utils::Packet::ptr&)>& handler)
{
    return this->packetReceivedHandlers.insert(handler);
}

unsigned int Sniffer::attachPacketReceivedHandler(std::function<void(const tin::utils::Packet::ptr&)>&& handler)
{
    return this->packetReceivedHandlers.insert(
        std::forward<std::function<void(const tin::utils::Packet::ptr&)>>(handler)
    );
}
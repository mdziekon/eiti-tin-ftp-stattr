#include "Sniffer.hpp"

using namespace tin::network::sniffer;

#include <iostream>

void pcap_trampoline(u_char *param, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
    Sniffer* obj = reinterpret_cast<Sniffer*>(param);
    obj->handlePacket(pkt_header, pkt_data);
}

Sniffer::Sniffer(const std::string& device, const std::string& expression)
{
    this->changeConfig(device, expression);
}

Sniffer::~Sniffer()
{
    if (this->isSniffing())
    {
        this->stopSniffing();
    }
    else if (this->snifferThread.joinable())
    {
        this->snifferThread.join();
    }
}

bool Sniffer::isSniffing() const
{
    return this->pcapHandle != nullptr;
}


void Sniffer::stopSniffing()
{
    if (!this->isSniffing())
    {
        return;
    }

    pcap_breakloop(this->pcapHandle);

    if (this->snifferThread.joinable())
    {
        this->snifferThread.join();
    }
}

void Sniffer::changeConfig(const std::string& device, const std::string& expression)
{
    if (this->isSniffing())
    {
        return;
    }

    std::string expressionBuilder = "(port ftp or port ftp-data)";

    this->device = std::string(device.c_str());
    if (expression.length() > 0)
    {
        expressionBuilder = expressionBuilder.append(std::string(" and (")).append(expression).append(")");
    }

    this->expression = std::string(expressionBuilder.c_str());
}

void Sniffer::handlePacket(const struct pcap_pkthdr *header, const u_char *packet)
{
    // declare pointers to packet headers
    const struct ethernet_header *ethernet;      // The ethernet header [1]
    const struct ip_header *ip;             // The IP header
    const struct tcp_header *tcp;           // The TCP header
    const struct pcaprec_header *pcap;      // The PCAP record header

    int size_ip;
    int size_tcp;
    int size_payload;

    this->packetCounter++;

    // define ethernet header
    ethernet = (struct ethernet_header*)(packet);
    pcap = (struct pcaprec_header*)(packet);

    long date = (long) pcap->ts_sec;
    time_t timestamp = time(&date);

    // define/compute ip header offset
    ip = (struct ip_header*) (packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20)
    {
        // Throw an error (Invalid IP header length: $size_ip bytes)
        return;
    }

    // if it is not TCP
    if(ip->ip_pro != IPPROTO_TCP)
    {
        auto pac = std::make_shared<tin::utils::Packet>(
            this->packetCounter,
            timestamp,
            ip->ip_src,
            ip->ip_dst,
            ip->ip_pro
        );

        this->runPacketReceivedHandlers(pac);

        // pac->printData();
        return;
    }

    // If it is TCP - we have some extra knowledge

    // Tcp header offset
    tcp = (struct tcp_header*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if (size_tcp < 20)
    {
        // throw an error (Invalid TCP header length: $size_tcp bytes)
        return;
    }

    // compute tcp payload (segment) size
    size_payload = ntohs(ip->ip_len) + (size_ip + size_tcp);

    // create new "Packet"
    auto pac = std::make_shared<tin::utils::Packet>(
        this->packetCounter,
        timestamp,
        ip->ip_src,
        ip->ip_dst,
        ip->ip_pro,
        tcp->th_sport,
        tcp->th_dport,
        size_payload
    );

    this->runPacketReceivedHandlers(pac);

    // pac->printData();
}

bool Sniffer::run()
{
    if (this->isSniffing())
    {
        return false;
    }
    if (this->snifferThread.joinable())
    {
        this->snifferThread.join();
    }

    if (this->initSniff())
    {
    	this->snifferThread = std::thread(
            &Sniffer::sniff,
            std::ref(*this)
        );

        return true;
    }

    return false;
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

void Sniffer::runPacketReceivedHandlers(const tin::utils::Packet::ptr& packetPtr)
{
    for (auto iter: this->packetReceivedHandlers)
    {
        iter.second(packetPtr);
    }
}

bool Sniffer::initSniff()
{
    if (this->isSniffing())
    {
        return false;
    }

    char errbuf[PCAP_ERRBUF_SIZE]; // error buffer
    auto& fp = this->compiledFilter;
    bpf_u_int32 mask;              // subnet mask
    bpf_u_int32 net;               // ip
    int num_packets = -1;          // number of packets to capture (-1 means till error) */


    // get device
    auto dev = this->device.c_str();

    if (dev == NULL)
    {
        // Throw an error (Couldn't find default device)
        std::cout << "[Sniffer] Could not start sniffer" << std::endl;
        return false;
    }


    // get network number and mask associated with capture device
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        // Throw an error (Couldn't get netmask for device: $device)
        net = 0;
        mask = 0;
    }

    // get an expression
    auto filter = this->expression.c_str();

    // open capture device
    this->pcapHandle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    auto& handle = this->pcapHandle;

    if (handle == NULL)
    {
        // Throw an error (Couldn't open device: $device)
        // errbuf contains error data
        std::cout << "[Sniffer] Could not start sniffer, no \"sudo\"?" << std::endl;
        return false;
    }

    // make sure we're capturing on an Ethernet device [2]
    if (pcap_datalink(handle) != DLT_EN10MB)
    {
        // throw an error ($device is not an Ethernet)
        return false;
    }

    // compile the filter
    if (pcap_compile(handle, &fp, filter, 0, net) == -1)
    {
        // throw an error (Could not parse filter: $expression)
        // pcap_geterr(handle)
        return false;
    }

    // apply the filter
    if (pcap_setfilter(handle, &fp) == -1)
    {
        // throw an error (Could not parse filter: $expression)
        // pcap_geterr(handle)
        return false;
    }

    return true;
}

void Sniffer::sniff()
{
    // capturing packets
    auto& handle = this->pcapHandle;
    auto& fp = this->compiledFilter;
    int num_packets = -1;

    pcap_loop(handle, num_packets, pcap_trampoline, reinterpret_cast<u_char*>(this));

    // cleanup
    pcap_freecode(&fp);
    pcap_close(handle);

    this->pcapHandle = nullptr;
}

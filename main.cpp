#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
using namespace std;

#define BUFFER_SIZE 65536

//4)convert protcol to human understandable string
string get_protocol(int protocol)
{
	switch(protocol){
		case 1: return "ICMP";
		case 6: return "TCP";
		case 17: return "UDP";
		default: return "OTHER";
	}
}
//3) extract ip header from packets
void process_packet(unsigned char* buffer, int size)
{
	struct iphdr* ip = (struct iphdr*)(buffer+sizeof(struct ethhdr));
	
	struct in_addr source,dest;
	source.s_addr = ip->saddr;
	dest.s_addr=ip->daddr;
	
	string protocol = get_protocol((int)ip->protocol);
	if(protocol=="OTHER")return;
	
	int ip_header_length=ip->ihl * 4;
	
	unsigned char* transport = (unsigned char*) ip+ip_header_length;
	
	int src_port =0;
	int dest_port =0;
	
	if(protocol == "TCP"){
		struct tcphdr*  tcp = (struct tcphdr*) transport;
		src_port = ntohs(tcp->source);
		dest_port = ntohs(tcp->dest);
	}
	else if (protocol == "UDP"){
		struct udphdr* udp = (struct udphdr*)transport;
		src_port = ntohs(udp->source);
		dest_port = ntohs(udp->dest);
	}
	
	
	cout << "Protocol: " << protocol<< "|" << "Source:  " << inet_ntoa(source) << ":"<< src_port << "|" 
	<< "Desination: "  << inet_ntoa(dest) << ":" << dest_port << endl;
	
}

int main()
{
	//1)create raw sockets
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	if(sock<0)
	{
		std::cerr <<"Failed to create socket"<<std::endl;
		return 1;
	}
	else
	{
		std::cout<<" Socket successfully created"<<std::endl;
	}
	
	
	unsigned char buffer[BUFFER_SIZE];
	while(true)
	{
		//2)capture packets
	 	int packet_size = recvfrom(sock,buffer,BUFFER_SIZE,0,nullptr,nullptr);
	 	
	 	if(packet_size < 0)
	 	{
	 		std :: cerr << "Failed to read packet." << std::endl;
	 	}
	 	
	 	std::cout <<"captured packet of size :" <<packet_size<< " bytes" <<std::endl;
	 	
	 	process_packet(buffer,packet_size);
	}
	
	close(sock);
	return 0;
}

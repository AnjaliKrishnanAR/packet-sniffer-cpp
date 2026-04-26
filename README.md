# Network Packet Sniffer

A command-line network packet capture tool built in C++ using raw sockets on Linux.

## Features
- Captures live TCP, UDP, and ICMP packets
- Displays source and destination IP addresses and ports
- Filters out irrelevant protocols
- Logs all captured packets with timestamps to capture.log

## Build
g++ main.cpp -o sniffer

## Run
sudo ./sniffer

## Requirements
Linux, g++, root privileges

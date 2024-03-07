#!/bin/bash

# attack_types=("LDAP" "MSSQL" "NetBIOS" "PortMap" "SYN" "UDP-Lag" "UDP")
attack_types=("MSSQL")
flow_packet_limit=6 # dump at most $flow_packet_limit packets per flow, any value less than 1 means no limit
# note that if you set a valid limit, you cannot get a realistic statistics of the flow

mkdir -p "../pcap/flow_pcap_5_tuple"

for attack_type in "${attack_types[@]}"; do
    echo "Splitting ${attack_type}"
    mkdir -p "../pcap/flow_pcap_5_tuple/${attack_type}"
    ../../../pcap_tool/splitter -i "../pcap/attack/${attack_type}.pcap" -o "../pcap/flow_pcap_5_tuple/${attack_type}" -f five_tuple -p $flow_packet_limit
done

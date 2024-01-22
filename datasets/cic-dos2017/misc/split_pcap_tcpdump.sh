pcap_file="AppDDos.pcap"
tcpdump -nr "${pcap_file}" -q | awk '{print $3, $5}' | seed 's/:[0-9]*//' | sort | uniq > ip_pairs_tcpdump.txt

# dir_name="split_tcpdump"
# mkdir -p "$dir_name"
# while IFS= read -r line; do
#     # Read the IP pair into variables
#     IFS=' ' read -r src_ip dst_ip <<< "$line"

#     # Remove the colon and port number from destination IP
#     dst_ip=${dst_ip%:*}

#     # Skip empty lines
#     if [ -z "$src_ip" ] || [ -z "$dst_ip" ]; then
#         continue
#     fi

#     # Create a filename based on the IP pair
#     filename="${dir_name}/${src_ip}_to_${dst_ip}.pcap"

#     # Use tcpdump to extract packets matching the IP pair
#     tcpdump -nr "${pcap_file}" -w "$filename" "ip src $src_ip and ip dst $dst_ip"
# done < ip_pairs_tcpdump.txt
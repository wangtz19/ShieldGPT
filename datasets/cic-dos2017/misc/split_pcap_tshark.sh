pcap_file="AppDDos.pcap"
tshark -r "${pcap_file}" -T fields -e ip.src -e ip.dst | sort | uniq > ip_pairs_tshark.txt

dir_name="split_tshark"
mkdir -p "$dir_name"
while IFS= read -r line; do
    # Read the IP pair into variables
    IFS=' ' read -r src_ip dst_ip <<< "$line"

    # Skip empty lines
    if [ -z "$src_ip" ] || [ -z "$dst_ip" ]; then
        continue
    fi

    # Create a filename based on the IP pair
    filename="${dir_name}/${src_ip}_to_${dst_ip}.pcap"

    # Use tshark to extract packets matching the IP pair
    tshark -r "${pcap_file}" -w "$filename" "ip.src==$src_ip and ip.dst==$dst_ip"
done < ip_pairs_tshark.txt

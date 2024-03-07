mkdir -p ../pcap/attack

# PortMap 9:43 - 9:51 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 12:43:00" -B "2018-11-03 12:51:59" ../pcap/first_day.pcap ../pcap/attack/PortMap.pcap

# NetBIOS 10:00 - 10:09 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 13:00:00" -B "2018-11-03 13:09:59" ../pcap/first_day.pcap ../pcap/attack/NetBIOS.pcap

# LDAP 10:21 - 10:30 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 13:21:00" -B "2018-11-03 13:30:59" ../pcap/first_day.pcap ../pcap/attack/LDAP.pcap

# MSSQL 10:33 - 10:42 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 13:33:00" -B "2018-11-03 13:42:59" ../pcap/first_day.pcap ../pcap/attack/MSSQL.pcap

# UDP 10:53 - 11:03 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 13:53:00" -B "2018-11-03 14:03:59" ../pcap/first_day.pcap ../pcap/attack/UDP.pcap

# UDP-Lag 11:14 - 11:24 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 14:14:00" -B "2018-11-03 14:24:59" ../pcap/first_day.pcap ../pcap/attack/UDP-Lag.pcap

# SYN 11:28 - 17:35 (UTC+5, CST-3)
editcap -F libpcap -A "2018-11-03 14:28:00" -B "2018-11-03 20:35:59" ../pcap/first_day.pcap ../pcap/attack/SYN.pcap
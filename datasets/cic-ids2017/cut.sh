mkdir -p flow_raw

# assure that the current timezone is UTC, otherwise proper time offset should be added

# Brute Force - FTP (2017-07-04 9:20 – 10:20 a.m.)
editcap -A "2017-07-04 09:20:00" -B "2017-07-04 10:20:00" Tuesday.pcap flow_raw/BruteForce-FTP.pcapng

# Brute Force - SSH (2017-07-04 14:00 – 15:00 p.m.)
editcap -A "2017-07-04 14:00:00" -B "2017-07-04 15:00:00" Tuesday.pcap flow_raw/BruteForce-SSH.pcapng

# DoS slowloris (9:47 – 10:10 a.m.)
editcap -A "2017-07-05 09:47:00" -B "2017-07-05 10:10:00" Wednesday.pcap flow_raw/DoS-Slowloris.pcapng

# DoS Slowhttptest (10:14 – 10:35 a.m.)
editcap -A "2017-07-05 10:14:00" -B "2017-07-05 10:35:00" Wednesday.pcap flow_raw/DoS-Slowhttptest.pcapng

# DoS Hulk (10:43 – 11 a.m.)
editcap -A "2017-07-05 10:43:00" -B "2017-07-05 11:00:00" Wednesday.pcap flow_raw/DoS-Hulk.pcapng

# DoS GoldenEye (11:10 – 11:23 a.m.)
editcap -A "2017-07-05 11:10:00" -B "2017-07-05 11:23:00" Wednesday.pcap flow_raw/DoS-GoldenEye.pcapng

# Heartbleed Port 444 (15:12 - 15:32)
editcap -A "2017-07-05 15:12:00" -B "2017-07-05 15:32:00" Wednesday.pcap flow_raw/Heartbleed-Port444.pcapng

# Web attack - Brute Force (2017-07-06 9:20-10 a.m.)
editcap -A "2017-07-06 09:20:00" -B "2017-07-06 10:00:00" Thursday.pcap flow_raw/Web-BruteForce.pcapng

# Web attack - XSS (2017-07-06 10:15-10:35 a.m.)
editcap -A "2017-07-06 10:15:00" -B "2017-07-06 10:35:00" Thursday.pcap flow_raw/Web-XSS.pcapng

# Web attack - Sql Injection (2017-07-06 10:40-10:42 a.m.)
editcap -A "2017-07-06 10:40:00" -B "2017-07-06 10:42:00" Thursday.pcap flow_raw/Web-SqlInjection.pcapng

# Infiltration – Dropbox download (2017-07-06 14:19-14:21 p.m. and 14:33 -14:35 and 15:04 – 15:45 p.m.)
editcap -A "2017-07-06 14:19:00" -B "2017-07-06 14:21:00" Thursday.pcap flow_raw/Infiltration-Dropbox-1.pcapng
editcap -A "2017-07-06 14:33:00" -B "2017-07-06 14:35:00" Thursday.pcap flow_raw/Infiltration-Dropbox-2.pcapng
editcap -A "2017-07-06 15:04:00" -B "2017-07-06 15:45:00" Thursday.pcap flow_raw/Infiltration-Dropbox-3.pcapng

# Infiltration – Cool disk (2017-07-06 14:53 – 15:00 p.m.)
editcap -A "2017-07-06 14:53:00" -B "2017-07-06 15:00:00" Thursday.pcap flow_raw/Infiltration-CoolDisk.pcapng

# Botnet ARES (10:02 a.m. – 11:02 a.m.)
editcap -A "2017-07-07 10:02:00" -B "2017-07-07 11:02:00" Friday.pcap flow_raw/Botnet-ARES.pcapng

# Port Scan (13:55 – 14:35, 14:51 - 15:29)
editcap -A "2017-07-07 14:01:00" -B "2017-07-07 14:04:00" Friday.pcap flow_raw/PortScan-RuleOn.pcapng
editcap -A "2017-07-07 14:51:00" -B "2017-07-07 15:29:00" Friday.pcap flow_raw/PortScan-RuleOff.pcapng

# DDoS LOIT (15:56 – 16:16)
editcap -A "2017-07-07 15:56:00" -B "2017-07-07 16:16:00" Friday.pcap flow_raw/DDoS-LOIT.pcapng

# cut a benign traffic
editcap -A "2017-07-07 20:00:00" -B "2017-07-07 20:05:00" Friday.pcap flow_raw/benign.pcapng
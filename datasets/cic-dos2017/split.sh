#! /bin/bash

splitter="../../pcap_tool/splitter"
"$splitter" -i "AppDDos.pcap" -o "flow_pcap" -f "ip_pair_reverse"

python split_appddos.py
python split_five_tuple.py
python split_train_test.py
python pcap2img.py
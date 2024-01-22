CIC Dos Dataset (2017) [web](https://www.unb.ca/cic/datasets/dos-dataset.html)

## Prepare
1. download the raw pcap file and rename it to `AppDDos.pcap`
2. split by ip pairs, annotate labels according to `attacks_dict.json`, split further by five tuples, and finally split train & test sets:
```sh
./split.sh
```
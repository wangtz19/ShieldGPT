A fast pcap splitter written with C++ by [Neptune](https://github.com/Neptune17/useless_libpcap_lib)

## Compile
Run the compiling script
```sh
./compile.sh
```
The compiled executable binary `splitter` is stored in this directory.

## Run
Run `./splitter -h` to get the following help information:
```text
Allowed options:
  -h [ --help ]            produce help message
  -i [ --input ] arg       set input file name
  -o [ --output_dir ] arg  set result directory
  -f [ --flow_format ] arg set format for splitting flows, available options: ip_pair, ip_pair_reverse, five_tuple
```

*flow format* explanation:
* `ip_pair`: the input pcap file is splitted by src & dst ip, and stored by `<src_ip>_<dst_ip>.pcap` format
* `ip_pair_reverse`: the input pcap file is splitted by src & dst ip, and stored by `<dst_ip>_<src_ip>.pcap` format
* `five_tuple`: the input pcap file is splitted by five tuple (ip, port & protocol), and stored by `<src_ip>_<src_port>_<proto>_<dst_ip>_<dst_port>.pcap` format
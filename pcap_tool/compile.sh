#! /bin/bash
set -x

# check whether current user is root, execute with sudo if not 
if [ $(id -u) != "0" ]; then
    prefix="sudo"
else
    prefix=""
fi

"$prefix" apt update
"$prefix" apt install -y libpcap-dev libboost-program-options-dev

# check whether g++ version is no less than 8
version=$(g++ -dumpversion | cut -f1 -d.)
if [ "$version" -lt 8 ]; then
    echo "g++ version is too low, install g++-8 ..."
    "$prefix" apt install -y g++-8
    "$prefix" update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 800
fi

cd pcap_split
make
mv main ../splitter
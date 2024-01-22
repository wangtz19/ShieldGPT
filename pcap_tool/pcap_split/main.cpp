#include <iostream>
#include <vector>
#include <map>
#include <boost/program_options.hpp>
#include <filesystem>

#include "abstract_feature_extractor.h"
#include "pcap_writer.h"
#include "pcap_reader.h"
#include "abstract_flow_identification.h"
#include "ip_pair_flow_identification.h"
#include "ip_pair_reverse_flow_identification.h"
#include "five_tuple_flow_identification.h"
#include "utils.h"

namespace po = boost::program_options;

int main(int argc, char *argv[]){
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input,i", po::value<std::string>(), "set input file name")
        ("output_dir,o", po::value<std::string>(), "set result directory")
        ("flow_format,f", po::value<std::string>(), "set format for splitting flows, available options: ip_pair, ip_pair_reverse, five_tuple")
        ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    std::string filename;
    if (vm.count("filename")) {
        filename = vm["filename"].as<std::string>();
    } else {
        std::cout << "Input filename was not set.\n";
        return 1;
    }

    std::string result_dir;
    if (vm.count("result_dir")) {
        result_dir = vm["result_dir"].as<std::string>();
        if (result_dir[result_dir.size() - 1] != '/')
            result_dir += '/';
        if (!std::filesystem::exists(result_dir)) {
            std::filesystem::create_directories(result_dir);
        }
    } else {
        std::cout << "Result directory was not set.\n";
        return 1;
    }

    std::string flow_format;
    if (vm.count("flow_format")) {
        flow_format = vm["flow_format"].as<std::string>();
    } else {
        flow_format = "ip_pair";
        std::cout << "Flow format was not set. Use default `ip_pair`\n";
    }

    PcapReader pcap_reader(filename.c_str(), false, false, true, false);

    std::map<std::string, PcapWriter*> pcap_writers;
    AbstractFlowIdentification<FiveTuple>* flow_identification;
    if (flow_format == "ip_pair")
        flow_identification = new IpPairFlowIdentification();
    else if (flow_format == "ip_pair_reverse")
        flow_identification = new IpPairReverseFlowIdentification();
    else if (flow_format == "five_tuple")
        flow_identification = new FiveTupleFlowIdentification();
    else{
        std::cout << "Flow format is not supported.\n";
        return 1;
    }

    clock_t start_time = clock();
    uint32_t pkt_cnt = 0;

    while(true){

        if (clock() - start_time > CLOCKS_PER_SEC){
            std::cout << pkt_cnt << std::endl;
            pkt_cnt = 0;
            start_time = clock();
        }

        pkt_cnt++;

        PktInfo pkt_info = pcap_reader.get_current_pkt_info();
        
        std::string flow_str = flow_identification->dump_flow_id(pkt_info.flow_id);

        if (pcap_writers.find(flow_str) == pcap_writers.end()){
            std::string cooked_filename = result_dir + flow_str + ".pcap";
            // std::cout << cooked_filename << std::endl;
            pcap_writers[flow_str] = new PcapWriter(cooked_filename.c_str());
        }

        u_char pkt_content[10000] = {};
        pcap_pkthdr pkt_header = pcap_pkthdr();
        pcap_reader.get_curr_original_pkt(pkt_content, &pkt_header);
        pcap_writers[flow_str]->dump_original_pkt(pkt_content, &pkt_header);

        pcap_reader.generate_next();
        if(pcap_reader.is_end())
            break;
    }
}
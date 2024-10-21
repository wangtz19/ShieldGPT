#include <iostream>
#include <vector>
#include <map>
#include <set>
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
        ("flow_packet_limit,l", po::value<int>(), "set the maximum number of packets for each flow")
        ("flow_prefix,p", po::value<std::string>(), "set the prefix for the flow file name")
        ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    std::string filename;
    if (vm.count("input")) {
        filename = vm["input"].as<std::string>();
    } else {
        std::cout << "Input filename was not set.\n";
        return 1;
    }

    std::string result_dir;
    if (vm.count("output_dir")) {
        result_dir = vm["output_dir"].as<std::string>();
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

    int flow_packet_limit = -1;
    if (vm.count("flow_packet_limit")) {
        flow_packet_limit = vm["flow_packet_limit"].as<int>();
        if (flow_packet_limit > 0) {
            std::cout << "Warning: Flow packet limit is set to " << flow_packet_limit << std::endl;
        }
    }

    std::string flow_prefix;
    if (vm.count("flow_prefix")) {
        flow_prefix = vm["flow_prefix"].as<std::string>();
    } else {
        flow_prefix = "";
    }
    PcapReader pcap_reader(filename.c_str(), false, false, true, false); // enable_original_pkt = true

    std::map<std::string, PcapWriter*> pcap_writers;
    // std::set<std::string> existing_flows;
    std::map<std::string, int> flow_packet_count;
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
    int total_pkt_cnt = 0;

    while(true){
        if (clock() - start_time > CLOCKS_PER_SEC){
            std::cout << "[# pkts in 1s] " << pkt_cnt << "  [total pkts] " << total_pkt_cnt << std::endl;
            pkt_cnt = 0;
            start_time = clock();
        }
        total_pkt_cnt++;
        pkt_cnt++;

        PktInfo pkt_info = pcap_reader.get_current_pkt_info();
        // std::cout << pkt_info << std::endl;
        
        std::string flow_str = flow_identification->dump_flow_id(pkt_info.flow_id, flow_prefix);
        bool is_new_flow = false;
        if (flow_packet_count.find(flow_str) == flow_packet_count.end()){
            flow_packet_count[flow_str] = 1;
            is_new_flow = true;
        } else if (flow_packet_limit > 0 && flow_packet_count[flow_str] >= flow_packet_limit) { // dump at most flow_packet_limit packets for each flow
            flow_packet_count[flow_str]++;

            if (pcap_writers.find(flow_str) != pcap_writers.end()){
                pcap_writers[flow_str]->close_dump_file();
                delete pcap_writers[flow_str];
                pcap_writers.erase(flow_str);
            }

            pcap_reader.generate_next();
            if(pcap_reader.is_end())
                break;
            continue;
        } else {
            flow_packet_count[flow_str]++;
        }
        if (pcap_writers.find(flow_str) == pcap_writers.end()){
            std::string cooked_filename = result_dir + flow_str + ".pcap";
            int data_link_type = pcap_reader.get_data_link_type();
            PcapWriter *ptr = new PcapWriter(cooked_filename.c_str(), 500, is_new_flow, data_link_type);
            // if (ptr->get_pcap_dumper() == NULL){
            //     // a simple fd scheduler with LRU policy
            //     // all file descriptors are used, close the first one thousand files
            //     clock_t st = clock();
            //     int cnt = 0;
            //     int max_cnt = 10;
            //     for (auto it = pcap_writers.begin(); it != pcap_writers.end();){
            //         it->second->close_dump_file();
            //         // delete it->second; // cancel recycling pointer for acceleration
            //         it = pcap_writers.erase(it);  // erase returns the iterator to the next element
            //         cnt++;
            //         if (cnt >= max_cnt)
            //             break;
            //     }
            //     ptr = new PcapWriter(cooked_filename.c_str(), 500, is_new_flow);
            //     assert (ptr->get_pcap_dumper() != NULL);
            //     std::cout << "time cost for LRU: " << (clock() - st) / (double)CLOCKS_PER_SEC << std::endl;
            // }
            pcap_writers[flow_str] = ptr;
        }

        u_char pkt_content[10000] = {};
        pcap_pkthdr pkt_header = pcap_pkthdr();
        pcap_reader.get_curr_original_pkt(pkt_content, &pkt_header);
        pcap_writers[flow_str]->dump_original_pkt(pkt_content, &pkt_header);

        pcap_reader.generate_next();
        if(pcap_reader.is_end()) {
            std::cout << "reach packet end" << std::endl;
            break;
        }
    }
}
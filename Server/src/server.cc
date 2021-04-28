//
// Created by dePaul Miller on 4/27/21.
//

#include <spdlog/spdlog.h>
#include <string>
#include <vector>
#include <Server.hh>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct ServerConf {
    unsigned clientPort;
    int rank;
    std::string address;
    std::vector<std::string> servers;

    ServerConf() {
        clientPort = 8081;
        rank = 0;
        address = "0.0.0.0";
        servers = {"127.0.0.1:8080"};
    }

    explicit ServerConf(const std::string &filename) {
        namespace pt = boost::property_tree;

        pt::ptree root;
        pt::read_json(filename, root);

        clientPort = root.get<unsigned>("clientPort", 8081);
        rank = root.get<int>("rank");
        address = root.get<std::string>("address", "0.0.0.0");

        for (auto &c : root.get_child("servers")) {
            servers.push_back(c.second.get<std::string>(""));
        }

    }

    ~ServerConf() = default;

};

[[noreturn]] int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::trace);

    ServerConf conf;

    if(argc == 2){
        conf = ServerConf(std::string(argv[1]));
        for(auto& i : conf.servers){
            std::cerr << i << std::endl;
        }
    }

    score::Server s(conf.rank, conf.servers, conf.address + ":" + std::to_string(conf.clientPort));

    while (true) {}
}
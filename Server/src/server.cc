//
// Created by dePaul Miller on 4/27/21.
//

#include <string>
#include <vector>
#include <Server.hh>

struct ServerConf {
    unsigned serverPort;
    unsigned clientPort;
    int rank;
    std::string address;
    std::vector<std::string> servers;

    ServerConf() {
        serverPort = 8080;
        clientPort = 8081;
        rank = 0;
        address = "0.0.0.0";
        servers = {"127.0.0.1"};
    }

    ~ServerConf() = default;

};

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);

    ServerConf conf;

    std::vector<std::string> internalAddr = conf.servers;

    for (auto & i : internalAddr) {
        i += ":" + std::to_string(conf.serverPort);
    }

    score::Server s(conf.rank, internalAddr, conf.address + ":" + std::to_string(conf.clientPort));

    while (true) {}

    return 0;
}
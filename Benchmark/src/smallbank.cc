//
// Created by dePaul Miller on 4/27/21.
//

#include <TxClientAPI.hh>
#include <boost/property_tree/ptree_fwd.hpp>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct ClientConf {
    unsigned port;
    std::string server;

    ClientConf() {
        port = 8081;
        server = "127.0.0.1";
    }

    explicit ClientConf(const std::string& filename) {
        namespace pt = boost::property_tree;
        pt::ptree root;

        pt::read_json(filename, root);

        port = root.get<unsigned>("port", 8081);
        server = root.get<std::string>("server", "127.0.0.1");
    }

    ~ClientConf() = default;

};

int main(int argc, char** argv){
    spdlog::set_level(spdlog::level::err);

    ClientConf conf;

    if(argc == 2){
        conf = ClientConf(std::string(argv[1]));
    }

    int accounts = 100;
    int start = 100;

    SPDLOG_INFO("Using server {}:{}", conf.server, conf.port);

    score::TxClient client(conf.server + ":" + std::to_string(conf.port));

    auto tx = client.StartTx();

    for(int i = 0; i < accounts; i++) {
        if (!tx.Write(std::to_string(i), std::to_string(start))) {
            SPDLOG_INFO("Failed initial population");
            return 1;
        }
        //std::cerr << "Wrote " << i << "," << start << std::endl;
    }

    if(!tx.TryCommit()){
        SPDLOG_INFO("Failed initial population");
        return 2;
    }

    SPDLOG_DEBUG("Committed initial writes");

    std::vector<std::thread> threads;

    auto stime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 4; i++){
        threads.emplace_back([&](){
            unsigned seed = time(nullptr);

            for(int i = 0; i < 100; i++) {
                auto tx = client.StartTx();
                unsigned userA = rand_r(&seed) % 100;
                unsigned userB = rand_r(&seed) % 100;
                while(userA == userB) {
                    userB = rand_r(&seed) % 100;
                }
                auto p = tx.Read(std::to_string(userA));
                if (!p.first) {
                    tx.TryCommit();
                    continue;
                }
                if(p.second.empty()){
                    tx.TryCommit();
                    continue;
                }
                auto p2 = tx.Read(std::to_string(userB));
                if (!p2.first) {
                    auto b = tx.TryCommit();
                    continue;
                }

                if(p2.second.empty()) {
                    auto b = tx.TryCommit();
                    continue;
                }

                if(std::stoi(p2.second) - 5 >= 0){
                    tx.Write(std::to_string(userA), std::to_string(std::stoi(p.second) + 5));
                    tx.Write(std::to_string(userB), std::to_string(std::stoi(p2.second) - 5));
                }

                if(tx.TryCommit()){
                    SPDLOG_DEBUG("Commit successful");
                }
            }
        });
    }

    for(auto& t : threads){
        t.join();
    }
    auto etime = std::chrono::high_resolution_clock::now();

    std::cout << "Throughput (Tx/s) " << 4 * 100 / std::chrono::duration<double>(etime - stime).count() << std::endl;

    auto tx2 = client.StartTx();

    int sum = 0;

    for(int i = 0; i < accounts; i++) {
        auto p = tx2.Read(std::to_string(i));
        if (!p.first) {
            return 4;
        }
        sum += std::stoi(p.second);
        //std::cerr << "Wrote " << i << "," << start << std::endl;
    }

    if(!tx2.TryCommit())
        return 5;

    if(sum != accounts * start) {
        std::cerr << "Weird balance " << sum << std::endl;
        return 6;
    }

    std::cout << "Worked" << std::endl;

    return 0;
}
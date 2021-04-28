//
// Created by dePaul Miller on 4/27/21.
//

#include <TxClientAPI.hh>
#include <boost/property_tree/ptree_fwd.hpp>
#include <spdlog/common.h>
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
    spdlog::set_level(spdlog::level::trace);

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

    for(int i = 0; i < 4; i++){
        threads.emplace_back([&](){
            unsigned seed = time(nullptr);

            for(int i = 0; i < 20; i++) {
                auto tx = client.StartTx();
                unsigned userA = 1;//rand_r(&seed) % 100;
                unsigned userB =  2;//rand_r(&seed) % 100;
                while(userA == userB) {
                    userB = rand_r(&seed) % 100;
                }
                auto p = tx.Read(std::to_string(userA));
                if (!p.first) {
                    tx.TryCommit();
                    continue;
                }
                auto p2 = tx.Read(std::to_string(userB));
                if (!p2.first) {
                    auto b = tx.TryCommit();
                    std::cout << b << " " << __LINE__ << std::endl;
                    continue;
                }

                if(p2.second.empty()) {
                    auto b = tx.TryCommit();
                    std::cout << b << " " << __LINE__ << std::endl;
                    continue;
                }

                bool updated = false;

                if(std::stoi(p2.second) - 5 >= 0){
                    tx.Write(std::to_string(userA), std::to_string(std::stoi(p.second) + 5));
                    tx.Write(std::to_string(userB), std::to_string(std::stoi(p2.second) - 5));

                    updated = true;
                    assert((std::stoi(p.second) + 5) + (std::stoi(p2.second) - 5) == std::stoi(p.second) + std::stoi(p2.second));
                }

                if(tx.TryCommit()){
                    if(updated)
                        SPDLOG_DEBUG("Changed {} -> {}, {} -> {} to {} -> {}, {} -> {}", userA, p.second, userB, p2.second,
                                 userA, std::stoi(p.second) + 5, userB, std::stoi(p2.second) - 5);
                }

                /*auto tx2 = client.StartTx();

                int sum = 0;

                for(int j = 0; j < accounts; j++) {
                    auto p3 = tx2.Read(std::to_string(j));
                    if (!p3.first) {
                        exit(4);
                    }
                    sum += std::stoi(p3.second);
                }

                if(!tx2.TryCommit())
                    exit(5);

                if(sum != accounts * start) {
                    std::cerr << "Weird balance " << sum << std::endl;
                    exit(6);
                }*/

            }
        });
    }

    for(auto& t : threads){
        t.join();
    }

    auto tx2 = client.StartTx();

    int sum = 0;

    for(int i = 0; i < accounts; i++) {
        auto p = tx2.Read(std::to_string(i));
        if (!p.first) {
            return 4;
        }
        SPDLOG_DEBUG("Read {} -> {}", i, p.second);
        sum += std::stoi(p.second);
        //std::cerr << "Wrote " << i << "," << start << std::endl;
    }

    if(!tx2.TryCommit())
        return 5;

    if(sum != accounts * start) {
        std::cerr << "Weird balance " << sum << std::endl;
            auto tx2 = client.StartTx();

            for(int i = 0; i < accounts; i++) {
                auto p = tx2.Read(std::to_string(i));
                if (!p.first) {
                    return 4;
                }
                SPDLOG_DEBUG("Read {} -> {}", i, p.second);
                sum += std::stoi(p.second);
                //std::cerr << "Wrote " << i << "," << start << std::endl;
            }

            if(!tx2.TryCommit())
                return 5;

            if(sum != accounts * start)
                std::cerr << "Still weird" << std::endl;

        return 6;
    }

    std::cout << "Worked" << std::endl;

    return 0;
}
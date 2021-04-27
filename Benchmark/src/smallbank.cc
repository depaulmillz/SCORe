//
// Created by dePaul Miller on 4/27/21.
//

#include <TxClientAPI.hh>
#include <string>
#include <thread>
#include <vector>
#include <spdlog/spdlog.h>

struct ClientConf {
    unsigned port;
    std::string server;

    ClientConf() {
        port = 8081;
        server = "127.0.0.1";
    }

    ~ClientConf() = default;

};

int main(int argc, char** argv){
    spdlog::set_level(spdlog::level::debug);

    ClientConf conf;

    int accounts = 100;
    int start = 100;

    std::cout << "Using server " << conf.server + ":" + std::to_string(conf.port) << std::endl;

    score::TxClient client(conf.server + ":" + std::to_string(conf.port));

    auto tx = client.StartTx();

    for(int i = 0; i < accounts; i++) {
        if (!tx.Write(std::to_string(i), std::to_string(start))) {
            return 1;
        }
        std::cerr << "Wrote " << i << "," << start << std::endl;
    }

    if(!tx.TryCommit())
        return 2;

    std::vector<std::thread> threads;

    for(int i = 0; i < 4; i++){
        threads.emplace_back([&](){
            unsigned seed = time(nullptr);

            for(int i = 0; i < 100; i++) {
                auto tx = client.StartTx();
                unsigned userA = rand_r(&seed) % 100;
                unsigned userB = rand_r(&seed) % 100;
                auto p = tx.Read(std::to_string(userA));
                if (!p.first) {
                    tx.TryCommit();
                    continue;
                }
                auto p2 = tx.Read(std::to_string(userB));
                if (!p2.first) {
                    tx.TryCommit();
                    continue;
                }

                if(p2.second == "") {
                    tx.TryCommit();
                    continue;
                }

                if(std::stoi(p2.second) - 5 >= 0){
                    tx.Write(std::to_string(userA), std::to_string(std::stoi(p.second) + 5));
                    tx.Write(std::to_string(userB), std::to_string(std::stoi(p2.second) - 5));
                }

                tx.TryCommit();
                std::cerr << "Pushed update" << std::endl;
            }
        });
    }

    for(auto& t : threads){
        t.join();
    }

    return 0;
}
//
// Created by dePaul Miller on 4/27/21.
//

#include <TxClientAPI.hh>
#include <string>
#include <thread>
#include <vector>
#include <spdlog/spdlog.h>
#include <unordered_map>

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

    std::unordered_map<int, int> assume;

    for(int i = 0; i < accounts; i++) {
        if (!tx.Write(std::to_string(i), std::to_string(start))) {
            return 1;
        }
        assume[i] = start;
        //std::cerr << "Wrote " << i << "," << start << std::endl;
    }

    if(!tx.TryCommit())
        return 2;

    SPDLOG_DEBUG("Committed initial writes");

    std::vector<std::thread> threads;

    for(int i = 0; i < 1; i++){
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

                if(std::stoi(p2.second) - 5 >= 0){
                    tx.Write(std::to_string(userA), std::to_string(std::stoi(p.second) + 5));
                    tx.Write(std::to_string(userB), std::to_string(std::stoi(p2.second) - 5));

                    SPDLOG_DEBUG("Changed {} -> {}, {} -> {} to {} -> {}, {} -> {}", userA, p.second, userB, p2.second,
                                 userA, std::stoi(p.second) + 5, userB, std::stoi(p2.second) - 5);

                    assert((std::stoi(p.second) + 5) + (std::stoi(p2.second) - 5) == std::stoi(p.second) + std::stoi(p2.second));
                }

                if(tx.TryCommit()){
                    SPDLOG_DEBUG("Commit successful");
                    assume[userA] += 5;
                    assume[userB] -= 5;
                }

                auto tx2 = client.StartTx();

                int sum = 0;

                for(int j = 0; j < accounts; j++) {
                    auto p3 = tx2.Read(std::to_string(j));
                    if (!p3.first) {
                        exit(4);
                    }
                    sum += std::stoi(p3.second);
                    assert(assume[j] == std::stoi(p3.second));
                }

                if(!tx2.TryCommit())
                    exit(5);

                if(sum != accounts * start) {
                    std::cerr << "Weird balance " << sum << std::endl;
                    exit(6);
                }

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
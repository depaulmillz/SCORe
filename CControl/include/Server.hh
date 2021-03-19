//
// Created by depaulsmiller on 3/18/21.
//

#include <thread>
#include <vector>
#include <iostream>
#include <CControlAPI.hh>
#include <CControlRPC.hh>
#include "gtest/gtest.h"
#include "protocol.pb.h"

#ifndef SCORE_SERVER_HH
#define SCORE_SERVER_HH

namespace score {
    class Server {
    public:
        Server(int rank, std::vector<std::string> addresses, std::string clientAddr) {
            CControlContextPair pair(rank, addresses.size());

            server = RunServer(addresses[rank], pair.cc);
            serverF = std::async([this]() {
                server->Wait();
            });

            std::vector<std::shared_ptr<InternalClient>> servers;
            for (auto addr : addresses) {
                servers.push_back(std::make_shared<InternalClient>(addr));
            }
            client = RunClientSideServer(clientAddr, pair.ctx, servers);
            clientF = std::async([this]() {
                client->Wait();
            });
        }

        void stop() {
            client->Shutdown();
            server->Shutdown();
            serverF.get();
            clientF.get();
        }

    private:
        std::future<void> serverF;
        std::future<void> clientF;
        std::unique_ptr<grpc::Server> server;
        std::unique_ptr<grpc::Server> client;
    };
}

#endif //SCORE_SERVER_HH
